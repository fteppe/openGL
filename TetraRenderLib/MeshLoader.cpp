#include "MeshLoader.h"

#include <functional>
using namespace tetraRender;

MeshLoader::MeshLoader(MeshContainer* meshesIn)
{
	meshes = meshesIn;
}


MeshLoader::~MeshLoader()
{
}

std::shared_ptr<Mesh> tetraRender::MeshLoader::getMesh(std::pair<std::string, std::string> meshName)
{
	objData* loaded = new objData;
	std::shared_ptr<Mesh> foundMesh = nullptr;
	//If this file has been loaded;
	if (meshes->find(meshName.first) != meshes->end())
	{
		auto meshIterator = meshes->find(meshName.first)->second;
		//and we find a mesh with that name
		if (meshIterator.find(meshName.second) != meshIterator.end())
		{
			foundMesh = meshIterator.find(meshName.second)->second;
		}
	}
	else
	{
		//If the loading is done we can grab the mesh
		if (checkFileLoadingProgress(meshName))
		{
			foundMesh = meshes->find(meshName.first)->second[meshName.second];
		}
	}
	return foundMesh;
}

GLfloat * tetraRender::MeshLoader::updateMesh(std::shared_ptr<Mesh> mesh, objData* objDataLoaded, unsigned meshId)
{
	tinyobj::shape_t* mesh_ptr = &objDataLoaded->shapes[meshId];
	unsigned indexOffset = 0;
	std::vector<std::vector<int>> polygons;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::map < std::vector < unsigned>, unsigned> indexTranslation;

	//Tiny obj has all the faces in a "flat" indices array. And num_face_vertices keeps track of how many vertices for each face. This makes the offset.
	for (unsigned face = 0; face < mesh_ptr->mesh.num_face_vertices.size(); face++)
	{
		unsigned polygonSize = mesh_ptr->mesh.num_face_vertices.size();
		std::vector<unsigned> polygon;
		for (int i = 0; i < polygonSize; i++)
		{
			unsigned translatedIndex;
			auto vertexObj = mesh_ptr->mesh.indices[indexOffset + i];
			unsigned vertexIndex = vertexObj.vertex_index;
			unsigned UVindex = vertexObj.texcoord_index;
			unsigned normalIndex = vertexObj.normal_index;
			//The issue is the difference in data format between openGL and waveFront .Obj; openGL a vertex data is the combination of all the elemeent (vertex, UV...)
			//Together, but Obj can have a vertex and it's norma with different indices. So we need as many UVs  as normals as vertices
			//two vertices can be shared between faces if and only if they have all the same attributes.
			//This is what index translation is for, we check if a vertex exists with all the attributes of an existing one. If it does we use
			//The existing one, otherwise we add it.
			if (indexTranslation.find({ vertexIndex, UVindex, normalIndex }) != indexTranslation.end())
			{
				translatedIndex = indexTranslation.find({ vertexIndex, UVindex, normalIndex })->second;
			}
			else
			{
				//We get the data for the vertex and add it at the end of our vertices.
				auto waveFrontVertices = objDataLoaded->attrib.vertices;
				glm::vec3 vertex = glm::vec3(waveFrontVertices[vertexIndex], waveFrontVertices[vertexIndex + 1], waveFrontVertices[vertexIndex + 2]);
				vertices.push_back(vertex);

				auto waveFrontUVs = objDataLoaded->attrib.texcoords;
				glm::vec2 UV = glm::vec2(waveFrontUVs[UVindex], waveFrontUVs[UVindex + 1]);
				UVs.push_back(vertex);

				auto waveFrontNormals = objDataLoaded->attrib.normals;
				glm::vec3 normal = glm::vec3(waveFrontNormals[normalIndex], waveFrontNormals[normalIndex + 1], waveFrontNormals[normalIndex + 2]);
				normals.push_back(vertex);
				
				//We are sure that there are always the same number of normal, vertex and UVs. the attributes of vertex i are vertices[i] normals[i] UVs[i]
				translatedIndex = vertices.size();
			}

			polygon.push_back(translatedIndex);
		}
	}

	return nullptr;
}

bool tetraRender::MeshLoader::checkFileLoadingProgress(std::pair<std::string, std::string> meshName)
{
	bool loadingDone = false;
	//The the file hasn't been loaded, we try to figure out if there is a task started out there to do it.
	if (fileLoadingFutures.find(meshName.first) != fileLoadingFutures.end())
	{
		//Since the task was started, we try to figure out if it is over or not.
		std::future<objData*>* future_ptr = &(fileLoadingFutures.find(meshName.first)->second);
		if (future_ptr->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			loadingDone = true;
			//The task of loading the file is over. It's now necessary to build the meshes.
			objData* loaded = future_ptr->get();
			createMeshUpdateTasks(loaded, meshName.first);
		}
	}
	return loadingDone;
}

void tetraRender::MeshLoader::createMeshUpdateTasks(objData * loaded, std::string fileName)
{
	std::vector<std::packaged_task<GLfloat*()>*> tasks;
	//We do in a single thread the work of updating all elements that have been loaded, but the meshes can know with
	//the future object if their update is done which can be before the end of the thread execution.
	for (unsigned i = 0; i < loaded->shapes.size(); i++)
	{
		tinyobj::shape_t* mesh_ptr = &loaded->shapes[i];
		std::shared_ptr<Mesh> newMesh;
		std::packaged_task<GLfloat*()>* updateMeshTask = new std::packaged_task<GLfloat*()>(std::bind(&MeshLoader::updateMesh, this, newMesh, loaded, i));
		newMesh->setDataFuture(std::move(updateMeshTask->get_future()));
		tasks.push_back(updateMeshTask);
		(meshes->find(fileName)->second)[mesh_ptr->name] = newMesh;
	}
	//Asynchronously all the tasks are done in a serie; This means few new threads are done 
	//and we avoid blocking the execution on the main thread.
	std::async([&tasks, loaded]() {
		for (int i = 0; i < tasks.size(); i++)
		{
			tasks[i]->operator()();
			delete tasks[i];
		}
		tasks.clear();
		delete loaded;
	});
}

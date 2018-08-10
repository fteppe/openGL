#include "MeshLoader.h"

#include <functional>

using namespace tetraRender;

tetraRender::MeshLoader::MeshLoader(ResourceAtlas& atlas) : atlas(atlas)
{
}


tetraRender::MeshLoader::~MeshLoader()
{
}

std::shared_ptr<Mesh> tetraRender::MeshLoader::getMesh(std::pair<std::string, std::string> meshName)
{
	objData* loaded = new objData;
	std::shared_ptr<Mesh> foundMesh = atlas.getMesh(meshName);
	//If this file has been loaded;
	if (foundMesh == nullptr)
	{

		//If the loading is done we can grab the mesh, but we need to make sure the loading was successfull. If it wasn't the mesh won't be available
		if (checkFileLoadingProgress(meshName.first).size())
		{
			foundMesh = atlas.getMesh(meshName);
		}
	}
	return foundMesh;
}

std::vector<std::shared_ptr<Mesh>> tetraRender::MeshLoader::getAllMeshes(std::string fileName)
{
	std::vector<std::shared_ptr<Mesh>> loadedMeshes;
	auto meshes = checkFileLoadingProgress(fileName);
	//The file is done loading.
	if (meshes.size());
	{
		for (MeshName mesh : meshes)
		{
			loadedMeshes.push_back(getMesh(mesh));
		}
	}
	return loadedMeshes;
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
		unsigned polygonSize = mesh_ptr->mesh.num_face_vertices[face];
		std::vector<int> polygon;
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
				glm::vec3 vertex = glm::vec3(waveFrontVertices[3 * vertexIndex], waveFrontVertices[3 * vertexIndex + 1], waveFrontVertices[3 * vertexIndex + 2]);
				vertices.push_back(vertex);

				auto waveFrontUVs = objDataLoaded->attrib.texcoords;
				glm::vec2 UV = glm::vec2(waveFrontUVs[2 * UVindex], waveFrontUVs[2 * UVindex + 1]);
				UVs.push_back(UV);

				auto waveFrontNormals = objDataLoaded->attrib.normals;
				glm::vec3 normal = glm::vec3(waveFrontNormals[3 * normalIndex], waveFrontNormals[3 * normalIndex + 1], waveFrontNormals[3 * normalIndex + 2]);
				normals.push_back(normal);
				//We are sure that there are always the same number of normal, vertex and UVs. the attributes of vertex i are vertices[i] normals[i] UVs[i]
				translatedIndex = vertices.size() -1 ;
				indexTranslation[{vertexIndex, UVindex, normalIndex}] = translatedIndex;
			}

			polygon.push_back(translatedIndex);
		}
		indexOffset += polygonSize;
		polygons.push_back(polygon);
		polygon.clear();
	}
	mesh->setVertexAndIndex(vertices, polygons);
	mesh->setNormals(normals);
	mesh->setUVs(UVs);
	mesh->findTangents();
	return mesh->createDataArray();
}

std::vector<MeshName> tetraRender::MeshLoader::checkFileLoadingProgress(std::string fileName)
{
	bool loadingDone = false;
	std::vector<MeshName> meshesLoaded;
	//We the file hasn't been loaded, we try to figure out if there is a task started out there to do it.
	if (fileLoadingFutures.find(fileName) != fileLoadingFutures.end())
	{
		//Since the task was started, we try to figure out if it is over or not.
		std::future<objData*>* future_ptr = &(fileLoadingFutures.find(fileName)->second);
		if (future_ptr->valid())
		{
			if (future_ptr->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				loadingDone = true;
				//The task of loading the file is over. It's now necessary to build the meshes.
				objData* loaded = future_ptr->get();
				meshesLoaded = createMeshUpdateTasks(loaded, fileName);
			}
		}
	}
	else {
		fileLoadingFutures[fileName] = std::async(&MeshLoader::loadFile, this, fileName);
	}
	return meshesLoaded;
}

objData * tetraRender::MeshLoader::loadFile(std::string fileName)
{
	std::vector<tinyobj::material_t> materials;
	std::string err;
	objData * fileObjContent = new objData;
	bool ret = tinyobj::LoadObj(&fileObjContent->attrib, &fileObjContent->shapes, &materials, &err, fileName.c_str());
	return fileObjContent;
}

std::vector<MeshName> tetraRender::MeshLoader::createMeshUpdateTasks(objData * loaded, std::string fileName)
{
	std::vector<std::packaged_task<GLfloat*()>*> tasks;
	std::vector<MeshName> meshesLoaded;
	//We do in a single thread the work of updating all elements that have been loaded, but the meshes can know with
	//the future object if their update is done which can be before the end of the thread execution.
	for (unsigned i = 0; i < loaded->shapes.size(); i++)
	{
		tinyobj::shape_t* mesh_ptr = &loaded->shapes[i];
		std::shared_ptr<Mesh> newMesh = std::shared_ptr<Mesh>( new Mesh);
		std::packaged_task<GLfloat*()>* updateMeshTask = new std::packaged_task<GLfloat*()>(std::bind(&MeshLoader::updateMesh, this, newMesh, loaded, i));
		newMesh->setDataFuture(std::move(updateMeshTask->get_future()));
		newMesh->setFilePath(MeshName(fileName, mesh_ptr->name));
		meshesLoaded.push_back(MeshName(fileName, mesh_ptr->name));
		tasks.push_back(updateMeshTask);
		atlas.addMesh(newMesh);
	}



	//Asynchronously all the tasks are done in a serie; This means few new threads are done 
	//and we avoid blocking the execution on the main thread.
	meshUpdateFutures.emplace_back(
		std::async([](std::vector<std::packaged_task<GLfloat*()>*> tasks, objData* loaded) {
		for (int i = 0; i < tasks.size(); i++)
		{
			tasks[i]->operator()();
			delete tasks[i];
		}
		tasks.clear();
		delete loaded;
	}, tasks, loaded));

	return meshesLoaded;
}



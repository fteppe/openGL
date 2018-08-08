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
	std::vector<std::packaged_task<GLfloat*()>*> tasks;
	//We do in a single thread the work of updating all elements that have been loaded, but the meshes can know with
	//the future object if their update is done which can be before the end of the thread execution.
	for (unsigned i = 0; i < loaded->shapes.size(); i++)
	{
		tinyobj::shape_t* mesh_ptr = &loaded->shapes[i];
		std::shared_ptr<Mesh> newMesh;
		
		std::packaged_task<GLfloat*()>* updateMeshTask = new std::packaged_task<GLfloat*()>(std::bind(&MeshLoader::updateMesh,this, newMesh, loaded, i));
		//newMesh.setfutureData(std::move(updateMeshTask->get_future()));
		tasks.push_back(updateMeshTask);
	}
	//Asynchronously all the tasks are done in a serie; This means few new threads are done 
	//and we avoid blocking the execution on the main thread.
	std::async([&tasks]() {
		for (int i = 0; i < tasks.size(); i++)
		{
			tasks[i]->operator()();
			delete tasks[i];
		}
		tasks.clear();
	});

	return nullptr;
}

GLfloat * tetraRender::MeshLoader::updateMesh(std::shared_ptr<Mesh> mesh, objData* objDataLoaded, unsigned meshId)
{
	tinyobj::shape_t* mesh_ptr = &objDataLoaded->shapes[meshId];
	return nullptr;
}

#include "stdafx.h"
#include "WaveFrontImporter.h"
#include <tetraRender\waveFrontLoader.h>
#include <tetraRender\Solid.h>

WaveFrontImporter::WaveFrontImporter(tetraRender::ResourceAtlas & atlas) : loader(tetraRender::MeshLoader(atlas))
{

}

void WaveFrontImporter::setDefaultMaterial(std::shared_ptr<tetraRender::Material> mat)
{
	defaultMaterial = mat;
}

void WaveFrontImporter::asyncLoad()
{
	loader.getAllMeshes(file);
	waitingLoad.emplace(file);
}

std::vector<tetraRender::GameObject*> WaveFrontImporter::getAsyncLoadResult()
{
	std::vector<tetraRender::GameObject*> objects;
	std::vector<std::string> filesDone;
	for (std::string loadingFile : waitingLoad)
	{
		auto meshes = loader.getAllMeshes(loadingFile);
		if (meshes.size())
		{
			tetraRender::GameObject* object = new tetraRender::GameObject();
			object->setName(loadingFile);
			for (auto mesh : meshes)
			{
				tetraRender::Solid* solid = new tetraRender::Solid(mesh);
				solid->addTag(RenderTag::WORLD_OBJECT);
				object->addChild(solid);
			}
			objects.push_back(object);
			filesDone.push_back(loadingFile);
		}
	}
	for (auto file : filesDone)
	{
		waitingLoad.erase(file);
	}
	return objects;
}

void WaveFrontImporter::setFile(std::string fileName)
{
	file = fileName;
}

std::string WaveFrontImporter::getFile()
{
	return file;
}


WaveFrontImporter::~WaveFrontImporter()
{
}

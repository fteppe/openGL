#pragma once
#include <tetraRender\Material.h>
#include <tetraRender\GameObject.h>
#include <tetraRender/MeshLoader.h>
#include <string>

class WaveFrontImporter
{
public:

	WaveFrontImporter(tetraRender::ResourceAtlas& atlas);
	void setDefaultMaterial(std::shared_ptr<tetraRender:: Material> mat);
	void asyncLoad();
	std::vector<tetraRender::GameObject*> getAsyncLoadResult();
	void setFile(std::string fileName);
	std::string getFile();
	~WaveFrontImporter();
	
private:
	std::string file;
	std::shared_ptr<tetraRender::Material> defaultMaterial;
	std::set<std::string> waitingLoad;
	tetraRender::MeshLoader loader;

};


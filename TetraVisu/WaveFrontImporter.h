#pragma once
#include <tetraRender\Material.h>
#include <tetraRender\GameObject.h>
#include <string>

class WaveFrontImporter
{
public:
	WaveFrontImporter();
	void setDefaultMaterial(std::shared_ptr<tetraRender:: Material> mat);
	tetraRender::GameObject* load();
	void setFile(std::string fileName);
	std::string getFile();
	~WaveFrontImporter();
	
private:
	std::string file;
	std::shared_ptr<tetraRender::Material> defaultMaterial;

};


#include "stdafx.h"
#include "WaveFrontImporter.h"
#include <tetraRender\waveFrontLoader.h>
#include <tetraRender\Solid.h>

WaveFrontImporter::WaveFrontImporter()
{
}

void WaveFrontImporter::setDefaultMaterial(std::shared_ptr<tetraRender::Material> mat)
{
	defaultMaterial = mat;
}

tetraRender::GameObject * WaveFrontImporter::load()
{
	tetraRender::WaveFrontLoader loader;
	std::vector<tetraRender:: Mesh* > meshes;
	loader.loadVertexObjectVectorFromFile(file, meshes);
	tetraRender::GameObject* root = new tetraRender::GameObject();
	root->setName(file);
	for (tetraRender::Mesh* mesh : meshes)
	{
		tetraRender::Solid* newSolid = new tetraRender::Solid(std::shared_ptr<tetraRender::Mesh>(mesh));
		newSolid->setMaterial(defaultMaterial);
		root->addChild(newSolid);
	}

	return root;
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

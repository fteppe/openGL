#include "ResourceAtlas.h"

using namespace tetraRender;

ResourceAtlas::ResourceAtlas()
{
}


ResourceAtlas::~ResourceAtlas()
{
}

std::string tetraRender::ResourceAtlas::addResource(AtlasContainer<Resource> container, std::shared_ptr<Resource> resource)
{
	std::string resourceName = resource->getName();
	std::string modifiedName = resourceName;
	int i = 1;
	//as long as there is a resource that has the same name we add numbers to the name
	while (container.find(modifiedName) != container.end())
	{
		modifiedName = resourceName + std::to_string(i);
		i++;
	}
	container.emplace(modifiedName, resource);
	return modifiedName;
}

std::string tetraRender::ResourceAtlas::addTexture(std::shared_ptr<Texture> tex)
{
	std::string resourceName = tex->getName();
	std::string modifiedName = resourceName;
	int i = 1;
	//as long as there is a resource that has the same name we add numbers to the name
	while (textures.find(modifiedName) != textures.end())
	{
		modifiedName = resourceName + std::to_string(i);
		i++;
	}
	tex->setName(modifiedName);
	textures.emplace(modifiedName, tex);
	return modifiedName;
}

std::shared_ptr<Texture> tetraRender::ResourceAtlas::getTexture(std::string texName)
{
	std::shared_ptr<Texture> returnVal = nullptr;
	if (textures.find(texName) != textures.end())
	{
		returnVal = textures[texName];
	}

	return returnVal;
}

std::string tetraRender::ResourceAtlas::addShader(std::shared_ptr<Shader> shader)
{
	std::string resourceName = shader->getName();
	std::string modifiedName = resourceName;
	int i = 1;
	//as long as there is a resource that has the same name we add numbers to the name
	while (shaders.find(modifiedName) != shaders.end())
	{
		modifiedName = resourceName + std::to_string(i);
		i++;
	}
	shader->setName(modifiedName);
	shaders.emplace(modifiedName, shader);
	return modifiedName;
}

std::shared_ptr<Shader> tetraRender::ResourceAtlas::getShader(std::string shaderName)
{
	std::shared_ptr<Shader> returnVal = nullptr;
	if (shaders.find(shaderName) != shaders.end())
	{
		returnVal = shaders[shaderName];
	}

	return returnVal;
}

std::string tetraRender::ResourceAtlas::addMaterial(std::shared_ptr<Material> mat)
{
	std::string resourceName = mat->getName();
	std::string modifiedName = resourceName;
	int i = 1;
	//as long as there is a resource that has the same name we add numbers to the name
	while (materials.find(modifiedName) != materials.end())
	{
		modifiedName = resourceName + std::to_string(i);
		i++;
	}
	mat->setName(modifiedName);
	materials.emplace(modifiedName, mat);
	return modifiedName;
}

std::shared_ptr<Material> tetraRender::ResourceAtlas::getMaterial(std::string materialName)
{
	return materials.find(materialName)->second;
}

const AtlasContainer<Texture>& tetraRender::ResourceAtlas::getTextures()
{
	return textures;
}

const AtlasContainer<Shader>& tetraRender::ResourceAtlas::getShaders()
{
	return shaders;
}

const AtlasContainer<Material>& tetraRender::ResourceAtlas::getMaterials()
{
	return materials;
}

const MeshContainer & tetraRender::ResourceAtlas::getMeshes()
{
	return meshes;
}

void tetraRender::ResourceAtlas::addMesh( std::shared_ptr<Mesh> mesh)
{
	std::pair<std::string, std::string> name = mesh->getFilePath();
	meshes[name.first][name.second] = mesh;
}

std::string tetraRender::ResourceAtlas::getUniqueGameObjectName(std::string gameObjectName)
{

	std::string modifiedName = gameObjectName;
	int i = 1;
	//as long as there is a resource that has the same name we add numbers to the name
	while (gameObjectNames.find(modifiedName) != gameObjectNames.end())
	{
		modifiedName = gameObjectName + std::to_string(i);
		i++;
	}
	gameObjectNames.emplace(modifiedName);
	return modifiedName;
}


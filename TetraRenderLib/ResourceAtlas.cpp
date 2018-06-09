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


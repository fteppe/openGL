#include "ParameterContainer.h"

using namespace tetraRender;

ParameterContainer::ParameterContainer()
{
}


ParameterContainer::~ParameterContainer()
{
}

void tetraRender::ParameterContainer::set(std::string valName, glm::vec3 val)
{
	vectors[valName] = val;
}

glm::vec3 tetraRender::ParameterContainer::getVec3(std::string valname)
{
	glm::vec3 val = glm::vec3(0);
	if (vectors.find(valname) != vectors.end())
	{
		val = vectors[valname];
	}
	return val;
}



#include "ParameterContainer.h"
#include "Common.h"

using namespace tetraRender;

ParameterContainer::ParameterContainer()
{
}


ParameterContainer::~ParameterContainer()
{
}

void tetraRender::ParameterContainer::set(std::string valName, glm::vec3 val)
{
	bool paramExists = false;
	for (auto param : parameters)
	{
		//It means that this parameter already exists, so no need to add it to the list.
		if (param == std::pair<std::string, tetraRender::ParameterType>(valName, ParameterType::VEC3))
		{
			paramExists = true;
		}
	}
	if (paramExists == false)
	{
		parameters.push_back(std::pair<std::string, tetraRender::ParameterType>(valName, ParameterType::VEC3));
	}
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

parametersList tetraRender::ParameterContainer::getParameters()
{
	return parameters;
}



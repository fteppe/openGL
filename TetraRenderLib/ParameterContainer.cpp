#include "ParameterContainer.h"
#include "Common.h"
#include "Resource.h"

using namespace tetraRender;

ParameterContainer::ParameterContainer()
{
}


ParameterContainer::~ParameterContainer()
{
}

void tetraRender::ParameterContainer::setResource(Resource * r)
{
	resource = r;
}

void tetraRender::ParameterContainer::set(std::string valName, glm::vec3 val)
{
	bool paramExists = checkParameterExistance(valName, ParameterType::VEC3);
	//if the parameter doesn't exist we add it to the list. If it already exist no need to change the list but we edit it's value.
	if (paramExists == false)
	{
		parameters.push_back(std::pair<std::string, tetraRender::ParameterType>(valName, ParameterType::VEC3));
	}
	vectors[valName] = val;
}

void tetraRender::ParameterContainer::set(std::string valName, std::string val)
{
	if (!checkParameterExistance(valName, ParameterType::STRING))
	{
		parameters.push_back(parameter(valName, ParameterType::STRING));
	}
	strings[valName] = val;
}

void tetraRender::ParameterContainer::set(std::string valName, float val)
{
	bool paramExists = checkParameterExistance(valName, ParameterType::FLOAT);
	//if the parameter doesn't exist we add it to the list. If it already exist no need to change the list but we edit it's value.
	if (paramExists == false)
	{
		parameters.push_back(std::pair<std::string, tetraRender::ParameterType>(valName, ParameterType::FLOAT));
	}
	floats[valName] = val;
}

void tetraRender::ParameterContainer::set(std::string valName, bool val)
{
	bool paramExists = checkParameterExistance(valName, ParameterType::BOOL);
	//if the parameter doesn't exist we add it to the list. If it already exist no need to change the list but we edit it's value.
	if (paramExists == false)
	{
		parameters.push_back(std::pair<std::string, tetraRender::ParameterType>(valName, ParameterType::BOOL));
	}
	bools[valName] = val;
}

bool tetraRender::ParameterContainer::getBool(std::string valName) const
{
	bool val = false;
	if (bools.find(valName) != bools.end())
	{
		val = bools.find(valName)->second;
	}
	return val;
}

glm::vec3 tetraRender::ParameterContainer::getVec3(std::string valname) const
{
	glm::vec3 val = glm::vec3(0);
	if (vectors.find(valname) != vectors.end())
	{
		val = vectors.find(valname)->second;
	}
	return val;
}

std::string tetraRender::ParameterContainer::getString(std::string valName) const
{
	std::string val = "";
	if (strings.find(valName) != strings.end())
	{
		val = strings.find(valName)->second;
	}
	return val;
}

float tetraRender::ParameterContainer::getFloat(std::string valName) const
{
	float val = 0;
	if (floats.find(valName) != floats.end())
	{
		val = floats.find(valName)->second;
	}
	return val;
}

std::vector<parameter> tetraRender::ParameterContainer::getParameters() const
{
	return parameters;
}

bool tetraRender::ParameterContainer::checkParameterExistance(std::string paramName, ParameterType type)
{
	bool paramExists = false;
	for (auto param : parameters)
	{
		//It means that this parameter already exists, so no need to add it to the list.
		if (param.first == paramName)
		{
			paramExists = true;
		}
	}

	return paramExists;
}



#pragma once
#include <string>
#include "ParameterContainer.h"

namespace tetraRender
{
	class Resource
{
public:
	Resource();
	~Resource();
	void setName(std::string name);
	std::string getName();
protected:
	std::string name;
	ParameterContainer parametersContainer;

};
}


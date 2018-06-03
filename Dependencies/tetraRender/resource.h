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
	ParameterContainer& getParameters();
	//This will use it's parameters to update itself accordingly.
	virtual void update();
protected:
	std::string name;
	ParameterContainer parametersContainer;

};
}


#include "Resource.h"

using namespace tetraRender;

const std::string Resource::nameField = "name";

Resource::Resource()
{
	parametersContainer.setResource(this);
}


Resource::~Resource()
{
}

void Resource::setName(std::string name)
{
	this->name = name;
	parametersContainer.set(nameField, name);
}

std::string Resource::getName()
{
	return parametersContainer.getString(nameField);
}

ParameterContainer& tetraRender::Resource::getParameters()
{
	return parametersContainer;
}

void tetraRender::Resource::update()
{
	setName(getName());
}

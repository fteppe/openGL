#include "Resource.h"

using namespace tetraRender;

const std::string nameField = "name";

Resource::Resource()
{
}


Resource::~Resource()
{
}

void Resource::setName(std::string name)
{
	parametersContainer.set(nameField, name);
}

std::string Resource::getName()
{
	return parametersContainer.getString(nameField);
}

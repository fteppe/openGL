#include "resource.h"

using namespace tetraRender;

Resource::Resource()
{
}


Resource::~Resource()
{
}

void Resource::setName(std::string name)
{
	this->name = name;
}

std::string Resource::getName()
{
	return name;
}

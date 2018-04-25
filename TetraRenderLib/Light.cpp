#include "stdafx.h"
#include "Light.h"
#include <iostream>
using namespace tetraRender;

Light::Light()
{

	_ASSERT(_CrtCheckMemory());
}

Light::Light(glm::vec3 pos, float intensity) : Light()
{
	this->intensity = intensity;
	this->pos = (pos);
	_ASSERT(_CrtCheckMemory());
}

std::vector<float> Light::getDataArray()
{
	std::vector<float> lightData({ pos.x, pos.y, pos.z, col.x, col.y, col.z, intensity });
	return lightData;
}

GameObjectType tetraRender::Light::getType() const
{
	return GameObjectType::LIGHT;
}




Light::~Light()
{
}

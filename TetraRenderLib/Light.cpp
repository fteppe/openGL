#include "stdafx.h"
#include "Light.h"
using namespace tetraRender;

Light::Light()
{
}

Light::Light(glm::vec3 pos, float intensity) : intensity(intensity)
{
	this->pos = (pos);
}

std::vector<float> Light::getDataArray()
{
	std::vector<float> lightData({ pos.x, pos.y, pos.z, col.x, col.y, col.z, intensity });
	return lightData;
}




Light::~Light()
{
}

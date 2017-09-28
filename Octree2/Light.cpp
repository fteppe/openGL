#include "stdafx.h"
#include "Light.h"


Light::Light()
{
}

Light::Light(glm::vec3 pos, float intensity) : pos(pos), intensity(intensity)
{
}

std::vector<float> Light::getDataArray()
{
	std::vector<float> lightData({ pos.x, pos.y, pos.z, col.x, col.y, col.z, intensity });
	return lightData;
}




Light::~Light()
{
}

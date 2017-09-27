#include "stdafx.h"
#include "Light.h"


Light::Light()
{
}

Light::Light(glm::vec3 pos, float intensity) : pos(pos), intensity(intensity)
{
}


Light::~Light()
{
}

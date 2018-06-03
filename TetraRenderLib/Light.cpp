#include "stdafx.h"
#include "Light.h"
#include <iostream>
using namespace tetraRender;
const std::string Light::col = "col";
const std::string Light::intensity = "intensity";

Light::Light()
{
	hasShadow = false;
	parametersContainer.set(intensity, 1);
}

Light::Light(glm::vec3 pos, float intensityIn) : Light()
{
	parametersContainer.set(col, glm::vec3(1));
	parametersContainer.set(intensity, intensityIn);
	setPos(pos);
}


void tetraRender::Light::setPos(glm::vec3 pos)
{
	GameObject::setPos(pos);
	if (hasShadow)
	{
		shadowProjection.setPos(pos);
	}
}

GameObjectType tetraRender::Light::getType() const
{
	return GameObjectType::LIGHT;
}




Light::~Light()
{
}

void tetraRender::Light::setProjection(glm::vec3 lookAt, glm::vec3 up)
{
	shadowProjection.setPos(getPos());
	shadowProjection.setUp(up);
	shadowProjection.setTarget(lookAt);
	hasShadow = true;
}

bool tetraRender::Light::getHasShadow()
{
	return hasShadow;
}

Camera * tetraRender::Light::getShadowProjection()
{
	return &shadowProjection;
}

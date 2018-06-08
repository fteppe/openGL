#include "stdafx.h"
#include "Light.h"
#include <iostream>
using namespace tetraRender;
const std::string Light::col = "col";
const std::string Light::intensity = "intensity";
const std::string Light::hasShadow = "hasShadow";

Light::Light()
{
	parametersContainer.set(hasShadow, false);
	parametersContainer.set(intensity, 1.0f);
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
	if (parametersContainer.getFloat(hasShadow))
	{
		shadowProjection.setPos(pos);
	}
}

GameObjectType tetraRender::Light::getType() const
{
	return GameObjectType::LIGHT;
}

void tetraRender::Light::update()
{
	GameObject::update();
	//this might seem a bit weird but we need to make sure that we properly update the position of the light respectively and the one of the shadowProjection.
	glm::vec3 pos = getPos();
	if (parametersContainer.getBool(hasShadow))
	{
		shadowProjection.setPos(pos);
	}
}




Light::~Light()
{
}

void tetraRender::Light::setProjection(glm::vec3 lookAt, glm::vec3 up)
{
	shadowProjection.setPos(getPos());
	shadowProjection.setUp(up);
	shadowProjection.setTarget(lookAt);
	parametersContainer.set(hasShadow, true);
}

bool tetraRender::Light::getHasShadow()
{
	return parametersContainer.getBool(hasShadow);
}

Camera * tetraRender::Light::getShadowProjection()
{
	return &shadowProjection;
}

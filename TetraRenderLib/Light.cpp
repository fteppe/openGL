#include "stdafx.h"
#include "Light.h"
#include <iostream>
using namespace tetraRender;

Light::Light()
{
	hasShadow = false;
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
	shadowProjection.setUp(up);
	shadowProjection.setTarget(lookAt);
}

bool tetraRender::Light::getHasShadow()
{
	return hasShadow;
}

Camera * tetraRender::Light::getShadowProjection()
{
	return &shadowProjection;
}

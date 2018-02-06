#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::setPos(glm::vec3 pos)
{
	this->pos = pos;
	modelMatrix = glm::translate(pos);
}

glm::vec3 GameObject::getPos()
{
	return pos;
}

glm::mat4 GameObject::getmodelMatrix() const
{
	return modelMatrix;
}

void GameObject::draw(Scene const & scene) const
{
}

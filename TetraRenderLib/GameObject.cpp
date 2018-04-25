#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"
#include <iostream>
using namespace tetraRender;

GameObject::GameObject()
{
	_ASSERT(_CrtCheckMemory());

	
	rotation = 0;
	pos = glm::vec3(0);
	scale = glm::vec3(1);
	rotationAngle = glm::vec3(0, 0, 1);
	updateModelMatrix();

	_ASSERT(_CrtCheckMemory());
}


GameObject::~GameObject()
{
}

void GameObject::setPos(glm::vec3 pos)
{
	this->pos = pos;
	updateModelMatrix();
}

void GameObject::setScale(glm::vec3 scale)
{
	this->scale = scale;
	updateModelMatrix();
}

void GameObject::setRotation(float rotation, glm::vec3 rotationAngle)
{
	this->rotation = rotation;
	this->rotationAngle = rotationAngle;
	updateModelMatrix();
}

void GameObject::addTag(RenderTag tag)
{
	renderTags.insert(tag);
}

void tetraRender::GameObject::addChild(GameObject * child)
{
	children.push_back(child);
}

void tetraRender::GameObject::setParent(GameObject * parent)
{
	this->parentNode = parent;
}

std::set<RenderTag> GameObject::getRenderTags()
{
	return renderTags;
}

glm::vec3 GameObject::getPos()
{
	return pos;
}

glm::vec3 GameObject::getScale()
{
	return scale;
}

glm::vec4 GameObject::getRotation()
{
	//TODO: see if this is right, might not be.
	return glm::vec4(rotationAngle, rotation);
}

glm::mat4 GameObject::getmodelMatrix() const
{
	GameObject* parent = this->parentNode;
	glm::mat4 mat = modelMatrix;
	while (parent != NULL)
	{
		mat = mat * parent->modelMatrix;
		parent = parent->parentNode;
	}
	return mat;
}


void GameObject::draw(tetraRender::Scene & scene)
{
	for (auto go : children)
	{
		go->draw(scene);
	}
}

void GameObject::draw(tetraRender::Scene & scene,std::shared_ptr<Material> mat)
{
	for (auto go : children)
	{
		go->draw(scene, mat);
	}
}

std::vector<GameObject*> tetraRender::GameObject::getChildren()
{
	return children;
}

void GameObject::updateModelMatrix()
{
	modelMatrix = glm::rotate(rotation, rotationAngle) * glm::scale(scale) *  glm::translate(pos);
}

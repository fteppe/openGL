#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"
#include <iostream>
using namespace tetraRender;

const std::string GameObject::pos = "pos";
const std::string GameObject::scale = "scale";
const std::string GameObject::rotationAngle = "rotAngle";

GameObject::GameObject()
{
	rotation = 0;
	setPos(glm::vec3(0));
	setScale(glm::vec3(1));
	setRotation(0, glm::vec3(0, 0, 1));
	updateModelMatrix();

}


GameObject::~GameObject()
{
}

void GameObject::setPos(glm::vec3 position)
{
	parametersContainer.set(this->pos, position);
	updateModelMatrix();
}

void GameObject::setScale(glm::vec3 scaleVec)
{
	parametersContainer.set(scale, scaleVec);
	updateModelMatrix();
}

void GameObject::setRotation(float rotation, glm::vec3 rotationAngleVec)
{
	this->rotation = rotation;
	parametersContainer.set(rotationAngle, rotationAngleVec);
	updateModelMatrix();
}

void GameObject::addTag(RenderTag tag)
{
	renderTags.insert(tag);
}

void tetraRender::GameObject::addChild(GameObject * child)
{
	child->setParent(this);
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
	return parametersContainer.getVec3(pos);
}

glm::vec3 GameObject::getScale()
{
	return parametersContainer.getVec3(scale);
}

glm::vec4 GameObject::getRotation()
{
	//TODO: see if this is right, might not be.
	return glm::vec4(parametersContainer.getVec3(rotationAngle), rotation);
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

GameObjectType tetraRender::GameObject::getType() const
{
	return GameObjectType::GAME_OBJECT;
}

std::vector<GameObject*> tetraRender::GameObject::getChildren()
{
	return children;
}

GameObject * tetraRender::GameObject::getParent()
{
	return parentNode;
}

void tetraRender::GameObject::update()
{
	updateModelMatrix();
}

void GameObject::updateModelMatrix()
{
	glm::vec3 posVec = parametersContainer.getVec3(pos);
	modelMatrix = glm::translate(getPos()) * glm::scale(getScale()) * glm::rotate(rotation, parametersContainer.getVec3(rotationAngle));
}

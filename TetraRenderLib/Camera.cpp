#include "stdafx.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <crtdbg.h>
#include <iostream>

using namespace tetraRender;

Camera::Camera()
{

}

Camera::Camera(float height, float width, float fov) : Camera()
{
	//begin and end, give how close and how far the camera renders.
	
	this->size = glm::vec2(width, height);
	nearPlane = 0.1f;
	farPlane = 200;
	projection = glm::perspective(fov, width / height, nearPlane, farPlane);
	//makeLookatMat();
}


GameObjectType tetraRender::Camera::getType() const
{
	return GameObjectType::CAMERA;
}

Camera::~Camera()
{


}
//this give the projection matrix to see through the camera.
glm::mat4 Camera::getProjection()
{
	return projection * lookAt;
}

void tetraRender::Camera::setProjectionOrtho(float height, float width, float near, float far)
{
	this->projection = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, near, far);
	nearPlane = near;
	farPlane = far;
}

void Camera::setLookAt(glm::mat4 lookAt)
{
	this->lookAt = lookAt;
}

void Camera::setPos(glm::vec3 pos)
{
	GameObject::setPos(pos);
	makeLookatMat();
}

void Camera::setTarget(glm::vec3 target)
{
	this->target = target;
	makeLookatMat();
}

void Camera::setUp(glm::vec3 up)
{
	this->up = up;
	makeLookatMat();
}

glm::vec3 Camera::getTarget()
{
	return target;
}

glm::vec3 Camera::getUp()
{
	return up;
}

glm::vec2 Camera::getSize()
{
	return size;
}

glm::vec2 Camera::getNearFarPlanes()
{
	return glm::vec2(nearPlane, farPlane);
}




void Camera::makeLookatMat()
{
	//glm::vec3 camPos = getmodelMatrix() * glm::vec4(1);
	this->lookAt.operator=(glm::lookAt(getPos(), target, up));
}

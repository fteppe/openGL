#include "stdafx.h"
#include "Camera.h"
#include <glm\gtx\transform.hpp>

Camera::Camera()
{
}

Camera::Camera(float height, float width, float fov)
{
	//begin and end, give how close and how far the camera renders.
	this->size = glm::vec2(width, height);
	nearPlane = 0.1f;
	farPlane = 200;
	projection = glm::perspective(fov, width / height, nearPlane, farPlane);
}

//this give the projection matrix to see through the camera.
glm::mat4 Camera::getProjection()
{
	return projection * lookAt;
}

void Camera::setLookAt(glm::mat4 lookAt)
{
	this->lookAt = lookAt;
}

void Camera::setPosition(glm::vec3 pos)
{
	this->pos = pos;
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

glm::vec3 Camera::getPos()
{
	return pos;
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




Camera::~Camera()
{
}

void Camera::makeLookatMat()
{
	lookAt = glm::lookAt(pos, target, up);
}

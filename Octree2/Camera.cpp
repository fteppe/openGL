#include "stdafx.h"
#include "Camera.h"
#include <glm\gtx\transform.hpp>

Camera::Camera()
{
}

Camera::Camera(float height, float width, float fov)
{
	//begin and end, give how close and how far the camera renders.
	float begin = 1.0f;
	float end = 500.0f;
	projection = glm::perspective(fov, width / height, begin, end);
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




Camera::~Camera()
{
}

void Camera::makeLookatMat()
{
	lookAt = glm::lookAt(pos, target, up);
}

#pragma once
#include <glm\matrix.hpp>
#include <glm\vec3.hpp>
class Camera
{
public:
	Camera();
	Camera(float height, float width, float fov);

	glm::mat4 getProjection();
	void setLookAt(glm::mat4 lookAt);
	void setPosition(glm::vec3 pos);
	void setTarget(glm::vec3 target);
	void setUp(glm::vec3 up);

	~Camera();
private:

	void makeLookatMat();

	//the matrix that has the informationson on it's position and orientation
	glm::mat4 lookAt;
	//The projection type of the camera.
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;

};


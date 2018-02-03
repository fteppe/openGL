#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
//Any object in the world. Goes with all the functions necessary to set coordinates, 
//rotation...
class GameObject
{
public:
	GameObject();
	~GameObject();
	void setPos(glm::vec3 pos);
	glm::vec3 getPos();
	glm::mat4 getmodelMatrix() const;
protected:
	glm::vec3 pos;
	glm::mat4 modelMatrix;
};


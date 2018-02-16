#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Scene;

//Any object in the world. Goes with all the functions necessary to set coordinates, 
//rotation...
class GameObject
{
public:
	GameObject();
	~GameObject();
	void setPos(glm::vec3 pos);
	void setScale(glm::vec3 scale);
	void setRotation(float rotation, glm::vec3 rotationAngle);
	glm::vec3 getPos();
	glm::vec3 getScale();
	glm::vec4 getRotation();
	glm::mat4 getmodelMatrix() const;
	virtual void draw(Scene& scene);
protected:
	void updateModelMatrix();
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotationAngle;
	float rotation;
	glm::mat4 modelMatrix;
};


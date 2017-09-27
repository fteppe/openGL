#pragma once
#include <glm\vec3.hpp>
class Light
{
public:
	Light();
	Light(glm::vec3 pos, float intensity);
	~Light();

	glm::vec3 pos;
	float intensity;
};


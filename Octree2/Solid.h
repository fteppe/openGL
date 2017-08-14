#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
#include <glm\matrix.hpp>
class Solid
{
public:
	Solid();
	Solid(std::vector < perso::Polygon> poly);
	~Solid();
	void draw();
private:
	std::vector<perso::Polygon> polygons;
	glm::mat4 objectSpace;
	bool triangulated;
	Shader shader;
};


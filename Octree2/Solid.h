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
	Solid(std::vector<glm::vec3> vertices, std::vector<std::vector<int>> index);
	~Solid();
	void draw();
	std::string description();
	void setObjectSpace(glm::mat4 transfo);
protected:
	void builVerticesIndex(std::vector<perso::Polygon> polygons);
	std::vector<perso::Polygon> polygons;
	std::vector<glm::vec3> vertices;
	std::vector<std::vector<int>> index;
	glm::mat4 objectSpace;
	bool triangulated;
	Shader shader;
};


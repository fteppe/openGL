#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include <glm\matrix.hpp>
class Solid
{
public:
	Solid();
	Solid(std::vector<glm::vec3> vertices, std::vector<std::vector<int>> index);
	~Solid();
	void draw(Camera cam, Light light);
	std::string description();
	void setObjectSpace(glm::mat4 transfo);
	void setNormals(std::vector<glm::vec3> normalIn);
protected:
	void builVerticesIndex(std::vector<perso::Polygon> polygons);
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<int>> index;
	glm::mat4 objectSpace;
	bool triangulated;
	Shader shader;
};


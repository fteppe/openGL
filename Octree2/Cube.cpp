#include "stdafx.h"
#include "Cube.h"


Cube::Cube(float side) : Solid(makePolys(side))
{
}

Cube::~Cube()
{
}

std::vector<perso::Polygon> Cube::makePolys(float side)
{
	float half = side / 2;
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(-half, half, half));
	points.push_back(glm::vec3(half, half, half));

	points.push_back(glm::vec3(half, -half, half));
	points.push_back(glm::vec3(-half, -half, half));


	points.push_back(glm::vec3(-half, half, -half));
	points.push_back(glm::vec3(half, half, -half));

	points.push_back(glm::vec3(half, -half, -half));
	points.push_back(glm::vec3(-half, -half, -half));


	std::vector<perso::Polygon> polys;
	//first polygon of the cube
	polys.push_back(perso::Polygon({points[0],points[1],points[2],points[3] }));
	polys.push_back(perso::Polygon({ points[0],points[3],points[7],points[4] }));
	polys.push_back(perso::Polygon({ points[3],points[2],points[6],points[7] }));
	polys.push_back(perso::Polygon({ points[4], points[5], points[6], points[7] }));
	polys.push_back(perso::Polygon({ points[1], points[2], points[6], points[5] }));
	polys.push_back(perso::Polygon({ points[0], points[1], points[5], points[4] }));

	return polys;
}

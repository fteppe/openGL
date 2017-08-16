#pragma once
#include "Vec3.h"
#include "Polygon.h"
#include "Solid.h"
#include <vector>
class Cube : public Solid
{
public:
	Cube(float side);
	~Cube();

private:
	std::vector<perso::Polygon> makePolys(float side);
	std::vector<std::vector<int>> makeIndex();
	std::vector<glm::vec3> makeVert(float side);

};


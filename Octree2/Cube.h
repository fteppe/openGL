#pragma once
#include "Vec3.h"
#include "Polygon.h"
#include <vector>
class Cube
{
public:
	Cube(Vec3 center, double half);
	~Cube();

private:
	std::vector<perso::Polygon> polys;
};


#include "stdafx.h"
#include "Solid.h"


Solid::Solid() : objectSpace() , polygons(), triangulated(false)
{
}

Solid::Solid(std::vector<perso::Polygon> poly) : objectSpace()
{
	//we take all the polygons and triangulize them
	for (int i = 0; i < poly.size(); i++)
	{
		std::vector<perso::Polygon> triangles = poly[i].triangleSplitting();
		//and we add the triangle polygons to the end of the vector of our polygons
		polygons.insert(polygons.end(), triangles.begin(), triangles.end());
	}
	
}


Solid::~Solid()
{
}

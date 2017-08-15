// Octree2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Polygon.h"
#include <iostream>
#include "Vec3.h"
#include "Octree.h"
#include "WindowBuilder.h"
#include "Shader.h"
void stuffs();

int main(int argc, char **argv)
{

	WindowBuilder window;


	//stuffs();
}

void stuffs()
{
	Octree tree = Octree(Vec3::zero(), 1.0);
	
	tree.subdivide();
	tree.add(glm::vec3(0.1f, 0.5f, -0.3f));
	std::cout << tree.description() << std::endl;
	glm::vec3 points[5] = { Vec3::u(), Vec3::u() + Vec3::w() ,Vec3::w() ,Vec3::u().multiply(-1) ,Vec3::w().multiply(-1) };
	perso::Polygon carre = perso::Polygon(std::vector<glm::vec3>(points, points + 5));
	std::vector<perso::Polygon> triangles = carre.triangleSplitting();
	for (int i = 0; i < triangles.size(); i++)
	{
		std::cout << triangles[i].description() << std::endl;
	}

}

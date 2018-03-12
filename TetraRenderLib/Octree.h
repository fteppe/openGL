#pragma once
#include <vector>
#include "Vec3.h"

class Octree
{
public:
	Octree();

	Octree(glm::vec3 center, double half);
	~Octree();
	/*
	The way the octree is built:
		bottom:
			45		/\
			67		|x
		top:		oy	->z
			01
			23
	*/
	//for any point returns in which quadrant in should be; See above.
	int getQuandrant(glm::vec3 point);
	//This function will add this point to either this tree, or the sub trees if they exist.
	void add(glm::vec3 point);
	//We create all the subtrees
	void subdivide();
	std::string description();
	glm::vec3 getCenter();
	double getHalf();
	bool contains(Vec3 point);

private:
	std::vector<Octree> subTree;
	std::vector<glm::vec3> elements;
	glm::vec3 center;
	double half;
};


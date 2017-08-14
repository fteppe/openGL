#include "stdafx.h"
#include "Octree.h"
#include <sstream>

Octree::Octree()
{
}

Octree::Octree(glm::vec3 center, double half)
{
	this->center = center;
	this->half = half;

}

Octree::~Octree()
{
}

/*
The way the octree is built:
	bottom:
		01
		23
				/\
				|x
	top:		oy	->z
		45
		67
*/
int Octree::getQuandrant(glm::vec3 point)
{
	int quadrant = 0;
	if (point.y > center.y)
	{
		quadrant += 4;
	}
	if (point.x < center.x)
	{
		quadrant += 2;
	}
	if (point.z > center.z)
	{
		quadrant += 1;
	}
	return quadrant;
}

void Octree::add(glm::vec3 point)
{
	//We make sure that the point is contained in the tree. If it out of bound it is not added.
	if (contains(point))
	{
		//if there is no subdivision
		if (subTree.size() == 0)
		{
			elements.push_back(point);
		}
		else
		{
			int quadrant = getQuandrant(point);
			subTree[quadrant].add(point);
		}
	}
}

void Octree::subdivide()
{
	//We will build the different quadrants
	double newHalf = half / 2;
	glm::vec3 newCenter = center + Vec3::u().multiply(newHalf) + Vec3::v().multiply(-newHalf) + Vec3::w().multiply(-newHalf);
	//We make the center of the first subdivision, and move it.
	//Remember that Y is VERTICAL. v is VERTICAL
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			subTree.push_back(Octree(newCenter, newHalf));
			newCenter = newCenter + Vec3::w().multiply(half);
			subTree.push_back(Octree(newCenter, newHalf));
			newCenter = newCenter + (Vec3::u() + Vec3::w()) * (float)(-half);
		}

		newCenter = newCenter + Vec3::v().multiply(half);
		newCenter = newCenter + (Vec3::u()).multiply(2*half);
	}
	while (elements.size())
	{
		add(elements.back());
		elements.pop_back();
	}
}

std::string Octree::description()
{
	std::stringstream ss;

	ss << "center " << Vec3(center) << std::endl << "half :" << half << std::endl;
	ss << "points:"<<std::endl;
	for (int i = 0; i < elements.size(); i++)
	{
		ss << elements[i] << std::endl;;
	}
	ss << "subtrees: " << std::endl;
	for (int i = 0; i < subTree.size(); i++)
	{
		ss << std::endl;
		ss << subTree[i].description();
	}
	return ss.str();
}

glm::vec3 Octree::getCenter()
{
	return center;
}

double Octree::getHalf()
{
	return half;
}

bool Octree::contains(Vec3 point)
{
	glm::vec3 relative = point + center*(float)(-1);
	bool isIn = relative.x >= -half && relative.x<=half && relative.y>=-half && relative.y <=half && relative.z>=-half && relative.z <= half;
	return isIn;}
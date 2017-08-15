#include "stdafx.h"
#include "Polygon.h"
#include <sstream>

using namespace perso;
Polygon::Polygon(std::vector<glm::vec3> pointsList)
{
	points = pointsList;
}


Polygon::~Polygon()
{
}

std::string Polygon::description()
{
	std::stringstream ss;
	ss << "points :";
	for (int i = 0; i < points.size(); i++)
	{
		glm::vec3 p = points[i];
		ss << p.x << " " << p.y << " " << p.z << " ; ";
	}
	return ss.str();
}

std::vector<Polygon> Polygon::triangleSplitting()
{
	return earSplitting();
}

std::vector<glm::vec3> perso::Polygon::getPoints()
{
	return points;
}

std::vector<GLfloat> perso::Polygon::getVertArray()
{
	std::vector<GLfloat> vertices;
	//we put all the coordinates in one GLfoat array.
	for (int i = 0; i < points.size(); i++)
	{
		vertices.push_back( points[i].x);
		vertices.push_back( points[i].y);
		vertices.push_back( points[i].z);
	}
	return vertices;
}

unsigned int perso::Polygon::getVertArraySize()
{
	return points.size() * 3 * sizeof(GLfloat);
}

std::vector<Polygon> Polygon::earSplitting()
{
	std::vector<Polygon> ears;
	int startPoint = 0;
	int size;
	while (points.size() > 3)
	{
		size = points.size();
		std::vector<glm::vec3> trianglePoints;
		trianglePoints.push_back(points[startPoint]);
		trianglePoints.push_back(points[(startPoint + 1) % size]);
		trianglePoints.push_back(points[(startPoint + 2) % size]);
		Polygon triangle = Polygon(trianglePoints);
		ears.push_back(triangle);
		//We remove the second point of the triangle
		points.erase(points.begin() + (startPoint + 1)%size);
		//To try and make the triangles more varied. They also should be smaller
		startPoint = (startPoint + 1) % size;
	}
	ears.push_back(Polygon(points));
	return ears;
}

bool Polygon::isInPolygon(std::vector<Vec3> triangle)
{
	return false;
}

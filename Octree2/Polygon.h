#pragma once
#include "Vec3.h"
#include <vector>
#include <string>
#include <glm\vec3.hpp>
#include <glew\glew.h>

namespace perso {
	class Polygon
	{
	public:
		Polygon(std::vector<glm::vec3> points);
		~Polygon();
		std::string description();
		//For now we assume the shape is concave.
		//will split the polygon in triangles, and will return an array of those triangles.
		std::vector<Polygon> triangleSplitting();
		//Takes a polygon, splits it in triangles and gives back the array of indexes for each face;
		//only works for flat, convex polygons
		static std::vector<std::vector<int>> triangleSplittingIndex(perso::Polygon, int offset);
		static std::vector<std::vector<int>> triangleSplittingIndex(std::vector<glm::vec3> points,std::vector<int> index, int offset);
		std::vector<glm::vec3> getPoints();
		std::vector<GLfloat> getVertArray();
		unsigned int getVertArraySize();
	private:
		std::vector<Polygon> earSplitting();
		bool isInPolygon(std::vector<Vec3> triangle);

		std::vector<glm::vec3> points;
	};
}



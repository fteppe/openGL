#pragma once
#include "Vec3.h"
#include <vector>
#include <string>
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
		const GLfloat* getVertArray();
	private:
		std::vector<Polygon> earSplitting();
		bool isInPolygon(std::vector<Vec3> triangle);

		std::vector<glm::vec3> points;
	};
}



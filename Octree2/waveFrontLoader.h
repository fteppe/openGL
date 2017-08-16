#pragma once
#include <vector>
#include <map>
#include <glm\vec3.hpp>
class WaveFrontLoader
{
public:
	WaveFrontLoader();
	~WaveFrontLoader();

private:
	void addVertexToPolygon(unsigned int vertex, unsigned int normal, unsigned int polygon);
	//For each vertex index, which normal is associated
	std::map<const unsigned int,unsigned int > vertexToNormal;
	//Since some vertices need to be cloned, we need to keep track of of what vertex is on the same position as another
	std::vector<std::vector<unsigned int>> vertexSynonyme;
	//The array of polygons that can be used to build a solid.
	std::vector<std::vector<unsigned int>> polygons;
	//the vertices
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
};


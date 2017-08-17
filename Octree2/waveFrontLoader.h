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
	//We give it the vertex and the normal that are together in the obj file. VertexFinal is the final index of the fertex after all our operations.
	//If it has been cloned then it's index changes to be given the nex vertex's index.
	//The map is to make the link between the vertices in OUR model and normals.
	//The synonyme 2D map is to link a vertex in the OBJ to all the cloned vertices that have the same coordinates but a different normal.
	//The array of vertices, is just that, the vertices from the OBJ file + all the cloned vertices resulting from our operations.
	void addVertexToPolygon(unsigned int vertex, unsigned int normal,
		int &vertexFinal, std::map<int, int> &vertexToNormal, std::map<int,std::vector<int>> &vertexSynonyme, std::vector<glm::vec3> &vertices);
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


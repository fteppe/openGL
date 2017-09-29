#pragma once
#include <vector>
#include <map>
#include <glm\vec3.hpp>
#include "Solid.h"
class WaveFrontLoader
{
public:
	WaveFrontLoader();
	~WaveFrontLoader();
	std::vector<Solid> GetSolidsFromFile(std::string fileName);

private:
	//We give it the vertex and the normal that are together in the obj file. VertexFinal is the final index of the fertex after all our operations.
	//If it has been cloned then it's index changes to be given the nex vertex's index.
	//The map is to make the link between the vertices in OUR model and normals.
	//The synonyme 2D map is to link a vertex in the OBJ to all the cloned vertices that have the same coordinates but a different normal.
	//The array of vertices, is just that, the vertices from the OBJ file + all the cloned vertices resulting from our operations.
	//return : the index that the vertex in the vertex/normal couple should have.
	int addVertexToPolygon(unsigned int vertex, unsigned int normal);

	//returns the index in the solid of the wavefront vertex index. If two different attributes have the same vertex, then we clone the vertex.
	int vertexAndAttributeLink(unsigned int vertex, unsigned int attribute, std::map<int,int>& vertexToAttribute);

	//Takes all the data that has been gather and transforms it to correspond to the Solid data model, and creates a Solid.
	Solid makeSolidFromData();
	std::map<int, int> fileToSolidVertexIndex;
	//For each vertex index, which normal is associated
	std::map<int,int > vertexToNormal;
	//For each vertex index, which normal is UVcoord
	std::map<int, int > vertexToUV;
	//Since some vertices need to be cloned, we need to keep track of of what vertex is on the same position as another
	std::map<int, std::vector<int>> vertexSynonyme;
	//The array of polygons that can be used to build a solid.
	std::vector<std::vector<int>> polygons;
	//the vertices of all the objets
	std::vector<glm::vec3> vertices;
	//the vertices but only for one solid;
	std::vector<glm::vec3> solidVertices;

	//the normals indexed as in the file
	std::vector<glm::vec3> normalsObj;
	//the UV coordinate indexed in the file
	std::vector<glm::vec3> UVobj;
};


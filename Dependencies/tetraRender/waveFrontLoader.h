#pragma once
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include "Solid.h"
namespace tetraRender
{
	class WaveFrontLoader
	{
	public:
		WaveFrontLoader();
		~WaveFrontLoader();
		void loadVertexObjectVectorFromFile(std::string filename, std::vector<VertexBufferObject*> &vertexObjects);
		VertexBufferObject* loadSpecificVBO(std::string fileName, std::string objectName);

	private:

		VertexBufferObject * loadObject();
		//returns the index in the solid of the wavefront vertex index. If two different attributes have the same vertex, then we clone the vertex.
		int vertexAndAttributeLink(unsigned int vertex, unsigned int attribute, std::map<int, int>& vertexToAttribute);

		//Takes all the data that has been gather and transforms it to correspond to the Solid data model, and creates a Solid.
		//Solid makeSolidFromData();

		//create an new VBO not owner of the object
		VertexBufferObject* makeVBOFromData();
		std::map<int, int> fileToSolidVertexIndex;
		//For each vertex index, which normal is associated
		std::map<int, int > vertexToNormal;
		//For each vertex index, which normal is UVcoord
		std::map<int, int > vertexToUV;
		//Since some vertices need to be cloned, we need to keep track of of what vertex is on the same position as another
		std::map<int, std::vector<int>> vertexSynonyme;
		//The array of polygons that can be used to build a solid.
		std::vector<std::vector<int>> polygons;
		//the vertices of all the objets
		std::vector<glm::vec3> vertices;
		unsigned int nbVerticesFile;
		//the vertices but only for one solid;
		std::vector<glm::vec3> solidVertices;

		//the normals indexed as in the file
		std::vector<glm::vec3> normalsObj;
		//the UV coordinate indexed in the file
		std::vector<glm::vec3> UVobj;
		//Which file is beeing loaded and which geometry inside it.
		std::pair<std::string, std::string> file;

	};


}
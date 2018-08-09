#pragma once
#include <glew/glew.h>
#include <vector>
#include "Shader.h"
//
//The vertex Buffer object contains all the geometry elements of an object.
namespace tetraRender
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn);
		~Mesh();
		/*
		*@param vertices a 2D array of series of n doubles, each n double representing a vertex. 1st line vertex coord and other lines vertex attributes
		*@param index indexes of the vertices to build the triangles (read it in sets of 3). ex: 1 2 3 1 4 5 2 3 5...
		*@param nbData for each row. So we can have different vector sizes for each attributes
		*/
		
		void drawObject(const Shader& shader);
		void setNormals(std::vector<glm::vec3> normalIn);
		void setUVs(std::vector<glm::vec2> UVin);

		void setFilePath(std::pair<std::string, std::string> filePath) ;
		std::pair<std::string, std::string> getFilePath() const;
		//THis function is used to find the tangent and bi-tangent that make the link between object coordinate 
		//and the UV coordinate.
		void findTangents();
		//if we add a new component to our object, we need to update the information on the GC
		void updateObjectAttributes();
		void setDataFuture(std::future<GLfloat*> future);

	private:

		void computeTangent(unsigned int P0, unsigned int P1, unsigned int P2);
		//This allows us to set the vertexAttributes of this object on the graphic card.		
		void setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData);
		void setVertex(GLfloat* verticesData, std::vector<int> attributesSize, std::vector<int> flatFaces);

		GLfloat* createDataArray();
		void asyncAttributesUpdate();


		int updateDataArray(GLfloat* dataArray, int offset, std::vector<glm::vec3>& datatoAdd);
		int updateDataArray(GLfloat* dataArray, int offset, std::vector<glm::vec2>& datatoAdd);

		//This member allows us to load data asynchronously
		std::future<GLfloat*> dataFuture;
		//This member tells us from which file, and which geometry in the file this was.
		std::pair <std::string, std::string> filePath;
		bool upToDate;
		unsigned int nbVertices;
		//For each attribute in this mesh we keep track of how many elements an attribute has: vertex has 3, UV has 2...
		std::vector<int> attributesSize;
		GLuint vertexbuffer;
		GLuint elementbuffer;
		GLuint VertexArrayID;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> UVs;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> biTangents;

		//A vector of vector of indices. It defines each face of the solid.
		std::vector<std::vector<int>> index;
	};
}


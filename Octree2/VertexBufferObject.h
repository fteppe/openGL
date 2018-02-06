#pragma once
#include "Dependencies\glew\glew.h"
#include <vector>
#include "Shader.h"
//The vertex Buffer object contains all the geometry elements of an object.
class VertexBufferObject
{
public:
	VertexBufferObject();
	VertexBufferObject(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn);
	~VertexBufferObject();
	/*
	*@param vertices a 2D array of series of n doubles, each n double representing a vertex. 1st line vertex coord and other lines vertex attributes
	*@param index indexes of the vertices to build the triangles (read it in sets of 3). ex: 1 2 3 1 4 5 2 3 5...
	*@param nbData for each row. So we can have different vector sizes for each attributes
	*/
	
	void drawObject(const Shader& shader);
	void setNormals(std::vector<glm::vec3> normalIn);
	void setUVs(std::vector<glm::vec3> UVin);
	//This allows us to set the vertexAttributes of this object on the graphic card.
	void setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData);
	void setFilePath(std::pair<std::string, std::string> filePath);
	//THis function is used to find the tangent and bi-tangent that make the link between object coordinate 
	//and the UV coordinate.
	void findTangents();

private:
	//if we add a new component to our object, we need to update the information on the GC
	void updateObjectAttributes();
	void computeTangent(unsigned int P0, unsigned int P1, unsigned int P2);
	//This member tells us from which file, and which geometry in the file this was.
	std::pair <std::string, std::string> filePath;

	unsigned int indexSize;
	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> UVs;

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> biTangents;
	//A vector of vector of indices. It defines each face of the solid.
	std::vector<std::vector<int>> index;
};


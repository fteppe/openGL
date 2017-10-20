#pragma once
#include "Dependencies\glew\glew.h"
#include <vector>
#include "Shader.h"
class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();
	/*
	*@param vertices a 2D array of series of n doubles, each n double representing a vertex. 1st line vertex coord and other lines vertex attributes
	*@param index indexes of the vertices to build the triangles (read it in sets of 3). ex: 1 2 3 1 4 5 2 3 5...
	*@param nbData for each row. So we can have different vector sizes for each attributes
	*/
	void setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData);
	void sendVertexToShader(const Shader& shader);

private:

	unsigned int indexSize;
	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;
};


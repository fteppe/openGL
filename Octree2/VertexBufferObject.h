#pragma once
#include "Dependencies\glew\glew.h"
#include <vector>
#include "Shader.h"
class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();
	void setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData);
	void sendVertexToShader(const Shader& shader);

private:

	unsigned int indexSize;
	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;
};


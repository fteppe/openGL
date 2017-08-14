#pragma once
#include <string>
#include <glew\glew.h>
#include <glm/matrix.hpp>
class Shader
{
public:
	Shader();
	Shader(std::string vertex, std::string fragment);
	~Shader();
	unsigned int getProgram();
	void setVertex(const GLfloat* vertices,unsigned int size);
	void draw();
	GLuint getvertexBuffer();
private:
	void compile();
	void compileVertex();
	void compileFragment();
	std::string sourceVertex;
	std::string sourceFragment;

	GLuint vertexbuffer;
	GLuint VertexArrayID;
	unsigned int verticesNum;
	unsigned int program;
	unsigned int vertexId;
	unsigned int fragmentId;
};


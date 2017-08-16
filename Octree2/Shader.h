#pragma once
#include <string>
#include <vector>
#include <glew\glew.h>
#include <glm/matrix.hpp>
class Shader
{
public:
	Shader();
	Shader(std::string vertex, std::string fragment);
	~Shader();
	unsigned int getProgram();
	void setVertex(std::vector<std::vector<GLfloat>> vertices,std::vector<int> index);
	void draw();
	GLuint getvertexBuffer();
private:
	void compile();
	void compileVertex();
	void compileFragment();
	std::string sourceVertex;
	std::string sourceFragment;

	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;
	unsigned int indexSize;
	unsigned int program;
	unsigned int vertexId;
	unsigned int fragmentId;
};


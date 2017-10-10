#pragma once
#include "Texture.h"
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
	void setVertex(std::vector<std::vector<GLfloat>> vertices,std::vector<int> index, std::vector<int> nbData);
	void setDiffuse(Texture tex);
	void draw();
	GLuint getvertexBuffer();

private:
	void compileShader(GLuint shader, std::string shaderPath);

	std::string sourceVertex;
	std::string sourceFragment;

	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;
	unsigned int indexSize;
	unsigned int program;
	Texture diffuse;
};


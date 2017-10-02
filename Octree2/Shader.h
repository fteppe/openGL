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
	void draw(std::vector<GLfloat> attributes, std::vector<std::vector<unsigned long long int>> attributesData, std::vector<int> faces);
	GLuint getvertexBuffer();
private:

	void compileShader(GLuint shader, std::string shaderPath);


	GLuint vertexbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;
	unsigned int indexSize;
	unsigned int program;
	unsigned int vertexId;
	unsigned int fragmentId;
	Texture diffuse;
};


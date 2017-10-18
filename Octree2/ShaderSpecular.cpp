#include "stdafx.h"
#include "ShaderSpecular.h"


ShaderSpecular::ShaderSpecular() : Shader("texture.ver","specular.frag")
{
	GLuint specCalc = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(specCalc, "shaders/specularCalc.frag");
	glAttachShader(program, specCalc);
	glDeleteShader(specCalc);
}


ShaderSpecular::~ShaderSpecular()
{
}

void ShaderSpecular::setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData)
{
	//we call the father function
	Shader::setVertex(vertices, index, nbData);
	//we just send to the program our new value
}

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



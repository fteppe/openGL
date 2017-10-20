#include "stdafx.h"
#include "ShaderSpecular.h"
#include "Camera.h"
#include "Solid.h"
#include <glm\gtc\type_ptr.hpp>



ShaderSpecular::ShaderSpecular() : Shader("texture.ver","specular.frag")
{
	//Link errors are expected since we first link without all the necessary files.
	GLuint specCalc = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(specCalc, "shaders/specularCalc.frag");
	glAttachShader(program, specCalc);
	glDeleteShader(specCalc);
	linkProgram();
}


ShaderSpecular::~ShaderSpecular()
{
}

void ShaderSpecular::setProgramInformation(const Scene & scene, const Solid & solid)
{
	Shader::setProgramInformation(scene, solid);
	Camera cam = scene.getCam();
	glm::vec3 camPos = cam.getPos();
	//sending the camera position
	glUniform3f(glGetUniformLocation(program, "camPos"), camPos.x,camPos.y,camPos.z);
}





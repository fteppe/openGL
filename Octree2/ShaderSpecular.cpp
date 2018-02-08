#include "stdafx.h"
#include "ShaderSpecular.h"
#include "Camera.h"
#include "Solid.h"
#include <glm/gtc/type_ptr.hpp>



ShaderSpecular::ShaderSpecular() : Shader({ "texture.ver" }, { "specular.frag","specularCalc.frag","lightCalc.frag" })
{
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
	if (uniforms.find("camPos") == uniforms.end())
	{
		uniforms["camPos"] = glGetUniformLocation(program, "camPos");
	}
	glUniform3f(uniforms["camPos"], camPos.x,camPos.y,camPos.z);
}





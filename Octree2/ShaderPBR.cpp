#include "stdafx.h"
#include "ShaderPBR.h"
#include "Camera.h"
#include "Scene.h"
#include "Solid.h"

ShaderPBR::ShaderPBR() : Shader({"texture.ver"}, { "PBR.frag","specularCalc.frag","lightCalc.frag" })
{
}

ShaderPBR::ShaderPBR(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders) : Shader(vertexShaders, fragmentShaders)
{
}


ShaderPBR::~ShaderPBR()
{
}

void ShaderPBR::setProgramInformation( Scene & scene, const Solid & solid)
{
	//We first send all the information that any shader has to send
	Shader::setProgramInformation(scene, solid);

	Camera cam = scene.getCam();
	glm::vec3 camPos = cam.getPos();
	glm::mat4 cameraSpace = cam.getProjection();
	glm::mat4 world2Obj = glm::inverse(solid.getmodelMatrix());
	sendMatrix4("world2obj", world2Obj);
	//sending the camera position
	if (uniforms.find("camPos") == uniforms.end())
	{
		uniforms["camPos"] = glGetUniformLocation(program, "camPos");
	}
	//std::cout << camPos.x<<" "<< camPos.y<<" "<< camPos.z << std::endl;
	glUniform3f(uniforms["camPos"], camPos.x, camPos.y, camPos.z);
	float time = scene.getElapsedTime();
	sendFloat("time", time);
}

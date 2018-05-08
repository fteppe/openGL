#include "ShaderPostProcess.h"
#include "Camera.h"
#include "Scene.h"
#include "Solid.h"
#include <sstream>

using namespace tetraRender;

ShaderPostProcess::ShaderPostProcess()
{
}

tetraRender::ShaderPostProcess::ShaderPostProcess(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders) : Shader(vertexShaders, fragmentShaders)
{
}


ShaderPostProcess::~ShaderPostProcess()
{
}

void tetraRender::ShaderPostProcess::setProgramInformation(tetraRender::Scene & scene, const Solid & solid)
{
	Shader::setProgramInformation(scene, solid);

	Camera cam = scene.getCam();
	glm::mat4 cameraSpace = cam.getProjection();
	sendMatrix4("inverseCam", glm::inverse(cameraSpace));

	float time = scene.getElapsedTime();
	sendFloat("time", time);

	std::vector<Light *> lights = scene.getLights();
	for (int i = 0; i < lights.size(); i++)
	{
		std::stringstream ss;
		ss << "lights[" << i << "]";
		sendLight(ss.str(), *lights[i]);
	}
	sendInt("numLights", lights.size());

	sendVec3("camPos", scene.getCam().getPos());
	
}

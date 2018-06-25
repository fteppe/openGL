#include "stdafx.h"
#include "ShaderPBR.h"
#include "Camera.h"
#include "Scene.h"
#include "Solid.h"

using namespace tetraRender;

ShaderPBR::ShaderPBR() : Shader({"texture.ver"}, { "PBR.frag","specularCalc.frag","lightCalc.frag" })
{
}

ShaderPBR::ShaderPBR(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders) : Shader(vertexShaders, fragmentShaders)
{
	getParameters().set("type", std::string("ShaderPBR"));
}


ShaderPBR::~ShaderPBR()
{
}

void ShaderPBR::setProgramInformation(tetraRender::Scene & scene, const Solid & solid)
{
	//We first send all the information that any shader has to send
	Shader::setProgramInformation(scene, solid);

	Camera cam = scene.getCam();

}

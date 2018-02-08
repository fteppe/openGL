#pragma once
#include "Shader.h"
//#include <vector>

class ShaderPBR :
	public Shader
{
public:
	ShaderPBR();
	ShaderPBR(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders);
	~ShaderPBR();

	void setProgramInformation(const Scene& scene, const Solid& solid);
};


#pragma once
#include "Shader.h"

namespace tetraRender {
	class ShaderPostProcess :public Shader
	{
	public:
		ShaderPostProcess();
		ShaderPostProcess(std::vector < std::string > vertex, std::vector<std::string> fragment);
		~ShaderPostProcess();

		void setProgramInformation(tetraRender::Scene & scene, const Solid & solid);
	};
}




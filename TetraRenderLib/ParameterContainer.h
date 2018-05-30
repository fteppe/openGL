#pragma once
#include <glm/vec3.hpp>
#include <map>
#include <vector>
#include <string>
#include "Common.h"

typedef std::vector<std::pair < std::string, tetraRender::ParameterType>> parametersList;

namespace tetraRender
{
	
	class ParameterContainer
	{
	public:
		ParameterContainer();
		~ParameterContainer();
		void set(std::string valName, glm::vec3 val);
		glm::vec3 getVec3(std::string valname);
		parametersList getParameters();

	private:
		std::map<std::string, glm::vec3> vectors;
		parametersList parameters;
	};
}



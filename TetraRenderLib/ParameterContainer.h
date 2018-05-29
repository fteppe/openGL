#pragma once
#include <glm/vec3.hpp>
#include <map>
#include <string>

namespace tetraRender
{
	class ParameterContainer
	{
	public:
		ParameterContainer();
		~ParameterContainer();
		void set(std::string valName, glm::vec3 val);
		glm::vec3 getVec3(std::string valname);

	private:
		std::map<std::string, glm::vec3> vectors;
	};
}



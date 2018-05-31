#pragma once
#include <glm/vec3.hpp>
#include <map>
#include <vector>
#include <string>
#include "Common.h"

typedef std::pair<std::string, tetraRender::ParameterType> parameter;


namespace tetraRender
{
	class Resource;
	class ParameterContainer
	{
	public:
		ParameterContainer();
		~ParameterContainer();
		void set(std::string valName, glm::vec3 val);
		glm::vec3 getVec3(std::string valname);
		std::vector<parameter> getParameters();

	private:
		std::map<std::string, glm::vec3> vectors;
		//This is a vector of all the parameters, It is a bad idea to put the value directly since a lot of polymorphism would be necessary.
		//it would also mean no primitive types.
		std::vector<parameter> parameters;
		//a parameter container has access to the resource it belongs to.
		//Resource* resource;
	};
}



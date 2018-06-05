#pragma once
#include <glm/vec3.hpp>
#include <map>
#include <vector>
#include <string>
namespace tetraRender
{
	class Resource;

}

#include "Common.h"
//#include "Resource.h"
typedef std::pair<std::string, tetraRender::ParameterType> parameter;


namespace tetraRender
{
	class ParameterContainer
	{
	public:
		ParameterContainer();
		~ParameterContainer();
		void setResource(Resource* r);
		void set(std::string valName, glm::vec3 val);
		void set(std::string valName, std::string val);
		void set(std::string valName, float val);
		void set(std::string valName, bool val);
		bool getBool(std::string valName) const;
		glm::vec3 getVec3(std::string valname) const;
		std::string getString(std::string valName) const;
		float getFloat(std::string valName) const;
		std::vector<parameter> getParameters() const;

	private:

		bool checkParameterExistance(std::string paramName, ParameterType type);
		std::map<std::string, float> floats;
		std::map<std::string, glm::vec3> vectors;
		std::map<std::string, std::string> strings;
		std::map<std::string, bool> bools;
		//This is a vector of all the parameters, It is a bad idea to put the value directly since a lot of polymorphism would be necessary.
		//it would also mean no primitive types.
		std::vector<parameter> parameters;
		//a parameter container has access to the resource it belongs to.
		Resource* resource;
	};
}



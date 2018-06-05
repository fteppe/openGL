#pragma once
#include <string>
#include "Scene.h"

namespace tetraRender
{
	class SceneSaver
	{
	public:
		SceneSaver();
		~SceneSaver();
		std::string toJson(Scene& scene);

		void addGameObjectToJSON(rapidjson::Writer<rapidjson::StringBuffer>& writer, GameObject* gameObject);
		void parameterToJSON(rapidjson::Writer<rapidjson::StringBuffer>& writer, ParameterContainer const & params);
	};
}



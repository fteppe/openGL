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
	};
}



#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <set>


enum RenderTag {
	RT_TRANSPARENT,
	RT_REFLECTIVE,
	STATIC,
	DYNAMIC,
	WORLD_OBJECT,
	POST_PROCESS
};

	class Go
	{
	public:
		Go();
		~Go();

	protected:
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotationAngle;
		glm::mat4 modelMatrix;

		//These tags are an indication for the renderer.
		std::set<RenderTag> renderTags;
	};




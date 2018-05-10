#pragma once
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "GameObject.h"

namespace tetraRender
{
	class GameObject;


	class Light : public GameObject
	{
	public:
		Light();
		Light(glm::vec3 pos, float intensity);
		//returns an array of size 7 with all the data of the light.
		//0-2 position,3-5 color, 6 intensity. 
		std::vector<float> getDataArray();
		virtual GameObjectType getType() const;
		~Light();

		glm::vec3 col;
		float intensity;
	};
}

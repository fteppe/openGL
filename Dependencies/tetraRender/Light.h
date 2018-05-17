#pragma once
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "GameObject.h"
#include "Camera.h"

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
		void setPos(glm::vec3 pos);
		virtual GameObjectType getType() const;
		~Light();

		void setProjection(glm::vec3 target, glm::vec3 up);
		bool getHasShadow();

		//This bool helps us know if we should get the projection from this light or not.
		bool hasShadow;
		//This is the matrix that is used to do the shadow projection in case it is necessary.
		//if it is null, then it means that this light doesn't project anything.
		Camera shadowProjection;
		glm::vec3 col;
		float intensity;
	};
}


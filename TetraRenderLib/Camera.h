#pragma once
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include "GameObject.h"

namespace tetraRender {
	class Camera : GameObject
	{
	public:
		Camera();
		Camera(float height, float width, float fov);

		glm::mat4 getProjection();
		void setProjectionOrtho(float height, float width, float near, float far);
		void setLookAt(glm::mat4 lookAt);
		void setPosition(glm::vec3 pos);
		void setTarget(glm::vec3 target);
		void setUp(glm::vec3 up);
		glm::vec3 getPos();
		glm::vec3 getTarget();
		glm::vec3 getUp();
		glm::vec2 getSize();
		glm::vec2 getNearFarPlanes();

		virtual GameObjectType getType() const;
		~Camera();

	private:

		void makeLookatMat();
		//the matrix that has the informationson on it's position and orientation
		glm::mat4 lookAt;
		//The projection type of the camera.
		glm::vec2 size;
		glm::mat4 projection;
		glm::vec3 target;
		glm::vec3 up;
		//glm::vec3 pos;

		float nearPlane;
		float farPlane;
	};
}



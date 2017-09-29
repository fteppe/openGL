#include "stdafx.h"
#include "Scene.h"
#include <glm\gtx\transform.hpp>

#include <glm\matrix.hpp>


Scene::Scene(Camera cam):cam(cam)
{
}

Scene::Scene(std::vector<Solid> elem, Camera cam) : Scene(cam)
{
	elements = elem;

	light.intensity = 1.0f;
	light.col = glm::vec3(1, 1, 1);
	light.pos = glm::vec3(3, 3, 0.5);
	cam.setPosition(glm::vec3(5, 5, 5));

}

void Scene::animate(sf::Clock elapsed)
{
	glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
	float rotation = elapsed.getElapsedTime().asMilliseconds();
	//std::cout << rotation << std::endl;
	glm::mat4 rot = glm::rotate(0.002f, glm::vec3(0, 0, 1));
	glm::vec3 pos = cam.getPos();
	//pos = rot * glm::vec4(pos , 1);
	light.pos = rot * glm::vec4(light.pos, 1);//;glm::vec4(5,0, 3,1);
	cam.setPosition(pos);
	cam.setTarget(glm::vec3(0, 0, 0));
	cam.setUp(glm::vec3(0, 0, 1));


}


Scene::~Scene()
{
}

void Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void Scene::renderScene()
{
	glm::mat4 rot(glm::rotate(0.5f, glm::vec3(1.0, 0, 0)));

	for (int i = 0; i < elements.size(); i++)
	{
		//elements[i].setObjectSpace(rot);
		elements[i].draw(cam, light);
	}
}
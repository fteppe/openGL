#include "stdafx.h"
#include "Scene.h"
#include <glm\gtx\transform.hpp>



Scene::Scene(Camera cam):cam(cam)
{
}

Scene::Scene(std::vector<Solid> elem, Camera cam) : Scene(cam)
{
	elements = elem;
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
		elements[i].setObjectSpace(rot);
		elements[i].draw(cam, light);
	}
}

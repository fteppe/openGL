#include "stdafx.h"
#include "Scene.h"


Scene::Scene() : cam(0.75f, 800.0f, 600.0f)
{
	
}

Scene::Scene(std::vector<Solid> elem) : elements(elem)
{
}


Scene::~Scene()
{
}

void Scene::setCamera(Camera &camera) 
{
	cam = (camera);
}

void Scene::renderScene()
{

	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].draw(cam, light);
	}
}

#include "stdafx.h"
#include "Scene.h"
#include <glm\gtx\transform.hpp>

#include <glm\matrix.hpp>


Scene::Scene(Camera cam):cam(cam)
{
}

Scene::Scene(std::vector<Solid> elem, Camera cam) : elements(elem), cam(cam)
{
	light.intensity = 1.0f;
	light.col = glm::vec3(1, 1, 1);
	light.pos = glm::vec3(1, 1, 0.2);
	this->cam.setPosition(glm::vec3(-10, -10, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 0, 1));
}

void Scene::animate(sf::Clock elapsed)
{
	glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
	float rotation = elapsed.getElapsedTime().asMilliseconds();
	//std::cout << rotation << std::endl;
	glm::mat4 rot = glm::rotate(0.02f, glm::vec3(0, 0, 1));
	glm::vec3 pos = cam.getPos();
	//pos = rot * glm::vec4(pos , 1);
	light.pos = rot * glm::vec4(light.pos, 1);//;glm::vec4(5,0, 3,1);



}


Scene::~Scene()
{
}

void Scene::eventHandler(sf::Event event)
{
	glm::vec3 campPos = cam.getPos();
	glm::vec3 target = cam.getTarget();
	glm::vec3 up = cam.getUp();
	glm::vec3 dir = target - campPos;
	float distance = glm::length(dir);
	dir = glm::normalize(dir);
	glm::vec3 perpendicular = glm::cross(dir, up);

	if (event.type == sf::Event::MouseMoved)
	{
		sf::Mouse mouse;
		glm::vec2 mousePos(mouse.getPosition().x, mouse.getPosition().y);
		glm::vec2 center(cam.getSize());
		mousePos = mousePos - center;
		target = target + up * mousePos.y;
		target += perpendicular * mousePos.x;
		//cam.setTarget(target);
		//we reset the mouse's position
		//mouse.setPosition(sf::Vector2i(0, 0));
	}
	if (event.type == sf::Event::KeyPressed)
	{

		sf::Keyboard keyboard;
		if (keyboard.isKeyPressed(keyboard.Z))
		{
			cam.setTarget(target + dir);
			cam.setPosition(campPos + dir);
		}
		if (keyboard.isKeyPressed(keyboard.S))
		{
			cam.setTarget(target - dir);
			cam.setPosition(campPos +- dir);
		}
		if (keyboard.isKeyPressed(keyboard.D))
		{
			
			cam.setTarget(target + perpendicular);
			cam.setPosition(campPos + perpendicular);
		}
		if (keyboard.isKeyPressed(keyboard.Q))
		{
			cam.setTarget(target - perpendicular);
			cam.setPosition(campPos - perpendicular);
		}
	}
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

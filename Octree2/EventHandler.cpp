#include "stdafx.h"
#include "EventHandler.h"


EventHandler::EventHandler()
{
}

EventHandler::EventHandler(std::weak_ptr<Scene> sceneIn)
{
	controlledScene = sceneIn;
	controlledObject = (controlledScene.lock().get()->elements).begin();
}


void EventHandler::handle(sf::Event eventIn)
{
	controlledScene.lock().get()->cam = camMouvement(eventIn);
	controlledObject = changeControlledObject(eventIn, controlledObject);
	//I get the pointer
	GameObject* obj = *controlledObject;
	moveGameObject(eventIn, obj);
}

EventHandler::~EventHandler()
{
}

Camera EventHandler::camMouvement(sf::Event event)
{
	Camera cam = controlledScene.lock().get()->cam;
	float speed = 0.1;
	glm::vec3 campPos = cam.getPos();
	glm::vec3 target = cam.getTarget();
	glm::vec3 up = cam.getUp();
	glm::vec3 dir = target - campPos;
	glm::vec3 dirNorm = glm::normalize(dir) * glm::vec3(speed);
	float distance = glm::length(dir);
	dir = glm::normalize(dir);
	glm::vec3 perpendicular = glm::cross(dir, up);

	//this way the angle when rotating doesn't depend on the distance between the camera and the target.
	perpendicular = perpendicular * (distance / 50);
	up = up * (distance / 50);
	if (event.type == sf::Event::KeyPressed)
	{
		//mouvements lat
		sf::Keyboard keyboard;
		if (keyboard.isKeyPressed(keyboard.Z))
		{
			target = (target + dirNorm);
			campPos = (campPos + dirNorm);
		}
		if (keyboard.isKeyPressed(keyboard.S))
		{
			target = (target - dirNorm);
			campPos = (campPos + -dirNorm);
		}
		if (keyboard.isKeyPressed(keyboard.D))
		{

			target = (target + speed * perpendicular);
			campPos = (campPos + speed * perpendicular);
		}
		if (keyboard.isKeyPressed(keyboard.Q))
		{
			target = target - speed * perpendicular;
			campPos -= speed * perpendicular;

		}
		if (keyboard.isKeyPressed(keyboard.Space))
		{
			target += speed * up;
			campPos +=  speed * up;
		}
		if (keyboard.isKeyPressed(keyboard.LControl))
		{
			target -= speed * up;
			campPos -= speed * up;
		}
		//mouv rot
		if (keyboard.isKeyPressed(keyboard.Left))
		{
			target = target - perpendicular;
		}
		if (keyboard.isKeyPressed(keyboard.Right))
		{
			target = target + perpendicular;
		}
		if (keyboard.isKeyPressed(keyboard.Down))
		{
			target = target - up;
		}
		if (keyboard.isKeyPressed(keyboard.Up))
		{
			target = target + up;
		}
	}
	cam.setTarget(target);
	cam.setPosition(campPos);
	return cam;
}



void EventHandler::moveGameObject(sf::Event event, GameObject * object)
{
	float speed = 0.1;
	glm::vec3 pos = object->getPos();
	if (event.type == sf::Event::KeyPressed)
	{
		//mouvements lat
		sf::Keyboard keyboard;
		if (keyboard.isKeyPressed(keyboard.Numpad8))
		{
			pos = pos + glm::vec3(speed, 0, 0);
		}
		if (keyboard.isKeyPressed(keyboard.Numpad2))
		{
			pos = pos + glm::vec3(-speed, 0, 0);
		}
		if (keyboard.isKeyPressed(keyboard.Numpad4))
		{
			pos = pos + glm::vec3(0, 0, speed);
		}
		if (keyboard.isKeyPressed(keyboard.Numpad6))
		{
			pos = pos + glm::vec3(0, 0, -speed);
		}
	}
	object->setPos(pos);
}

std::vector<GameObject*>::iterator EventHandler::changeControlledObject(sf::Event event , std::vector<GameObject*>::iterator& it)
{
	if (event.type == sf::Event::KeyPressed)
	{
		std::vector<GameObject*>* elems = &controlledScene.lock().get()->elements;
		//mouvements lat
		sf::Keyboard keyboard;

		if (keyboard.isKeyPressed(keyboard.PageDown))
		{
			if (it + 1 == elems->end())
			{
				it = elems->begin();
			}
			else
			{
				it++;
			}
		}
		if (keyboard.isKeyPressed(keyboard.PageUp))
		{
			if (it == elems->begin())
			{
				it = elems->end() - 1;
			}
			else
			{
				it--;
			}
		}

	}
	return it;
}

#include "stdafx.h"
#include "EventHandler.h"
#include <iostream>

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(std::weak_ptr<tetraRender::Scene> sceneIn)
{
	controlledScene = sceneIn;
	//controlledObject = (controlledScene.lock().get()->gameObjects).begin();
}


void EventHandler::handle(SDL_Event eventIn)
{
	controlledScene.lock().get()->setCamera( camMouvement(eventIn));
	//controlledObject = changeControlledObject(eventIn, controlledObject);
	//I get the pointer

}

EventHandler::~EventHandler()
{
}

tetraRender:: Camera EventHandler::camMouvement(SDL_Event event)
{
	tetraRender::Camera  cam = controlledScene.lock().get()->getCam();
	float speed = 0.1f;
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
	if (event.type == SDL_KEYDOWN)
	{
		//mouvements lat
		auto key = event.key.keysym.sym;
		if (key == SDLK_z)
		{

			target = (target + dirNorm);
			campPos = (campPos + dirNorm);
		}
		if (key == SDLK_s)
		{
			target = (target - dirNorm);
			campPos = (campPos + -dirNorm);
		}
		if (key == SDLK_d)
		{

			target = (target + speed * perpendicular);
			campPos = (campPos + speed * perpendicular);
		}
		if (key == SDLK_q)
		{
			target = target - speed * perpendicular;
			campPos -= speed * perpendicular;

		}
		if (key == SDLK_SPACE)
		{
			target += speed * up;
			campPos +=  speed * up;
		}
		if (key == SDLK_LCTRL)
		{
			target -= speed * up;
			campPos -= speed * up;
		}
		//mouv rot
		if (key == SDLK_LEFT)
		{
			target = target - perpendicular;
		}
		if (key == SDLK_RIGHT)
		{
			target = target + perpendicular;
		}
		if (key == SDLK_DOWN)
		{
			target = target - up;
		}
		if (key == SDLK_UP)
		{
			target = target + up;
		}
	}
	cam.setTarget(target);
	cam.setPos(campPos);
	return cam;
}



//void EventHandler::moveGameObject(sf::Event event, GameObject * object)
//{
//	float speed = 0.1f;
//	glm::vec3 pos = object->getPos();
//	if (event.type == sf::Event::KeyPressed)
//	{
//		//mouvements lat
//		sf::Keyboard keyboard;
//		if (keyboard.isKeyPressed(keyboard.Numpad8))
//		{
//			pos = pos + glm::vec3(speed, 0, 0);
//		}
//		if (keyboard.isKeyPressed(keyboard.Numpad2))
//		{
//			pos = pos + glm::vec3(-speed, 0, 0);
//		}
//		if (keyboard.isKeyPressed(keyboard.Numpad4))
//		{
//			pos = pos + glm::vec3(0, 0, speed);
//		}
//		if (keyboard.isKeyPressed(keyboard.Numpad6))
//		{
//			pos = pos + glm::vec3(0, 0, -speed);
//		}
//	}
//	object->setPos(pos);
//}
//
//std::vector<GameObject*>::iterator EventHandler::changeControlledObject(sf::Event event , std::vector<GameObject*>::iterator& it)
//{
//	if (event.type == sf::Event::KeyPressed)
//	{
//		std::vector<GameObject*>* elems = &controlledScene.lock().get()->gameObjects;
//		//mouvements lat
//		sf::Keyboard keyboard;
//
//		if (keyboard.isKeyPressed(keyboard.PageDown))
//		{
//			if (it + 1 == elems->end())
//			{
//				it = elems->begin();
//			}
//			else
//			{
//				it++;
//			}
//		}
//		if (keyboard.isKeyPressed(keyboard.PageUp))
//		{
//			if (it == elems->begin())
//			{
//				it = elems->end() - 1;
//			}
//			else
//			{
//				it--;
//			}
//		}
//
//	}
//	return it;
//}

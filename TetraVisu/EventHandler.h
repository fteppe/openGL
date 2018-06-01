#pragma once
#include <tetraRender/Scene.h>
//#include <SFML/OpenGL.hpp>
//#include <SFML/Window.hpp>
#include <SDL2/SDL.h>

class EventHandler
{
public:
	EventHandler();
	EventHandler(std::weak_ptr<tetraRender:: Scene> sceneIn);

	void handle(SDL_Event eventIn);
	~EventHandler();
private:
	tetraRender::Camera camMouvement(SDL_Event event);
	/*
	void moveGameObject(sf::Event event, GameObject* object);
	std::vector<GameObject *>::iterator changeControlledObject(sf::Event, std::vector<GameObject * >::iterator& it);*/
	std::weak_ptr<tetraRender:: Scene> controlledScene;
	std::vector<tetraRender::GameObject *>::iterator controlledObject;
};



#pragma once
#include "Scene.h"
#include <SFML\OpenGL.hpp>

class EventHandler
{
public:
	EventHandler();
	EventHandler(std::weak_ptr<Scene> sceneIn);

	void handle(sf::Event eventIn);
	~EventHandler();
private:
	Camera camMouvement(sf::Event event);
	void moveGameObject(sf::Event event, GameObject* object);
	std::vector<Solid>::iterator changeControlledObject(sf::Event, std::vector<Solid>::iterator& it);
	std::weak_ptr<Scene> controlledScene;
	std::vector<Solid>::iterator controlledObject;
};


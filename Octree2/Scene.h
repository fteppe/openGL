#pragma once
#include "Camera.h"
#include "Light.h"
#include "Solid.h"
#include "SceneLoader.h"
#include <vector>
#include <SFML/Window.hpp>
/*
This class contains objects, a light and a camera. So we should be able to render a scene.
*/
class Scene
{
	friend class EventHandler;
public:
	Scene(Camera cam);
	Scene(std::vector<Solid> elem, Camera cam);
	void animate(sf::Clock elapsed);
	~Scene();
	void eventHandler(sf::Event event);
	void setCamera(Camera camera);
	void renderScene();

	Camera getCam() const;
	Light getLight() const;
private:
	Camera cam;
	Light light;
	std::vector<Solid> elements;
	SceneLoader loader;
};


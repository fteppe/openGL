#pragma once
#include <SFML/Window.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <tetraRender/Scene.h>
//
class WindowBuilder
{
public:
	WindowBuilder();
	WindowBuilder(std::string scene);
	~WindowBuilder();

	void draw();

private:
	sf::RenderWindow window;
	std::shared_ptr<tetraRender::Scene> scene;
	sf::Clock clock;
	sf::Clock deltaClock;
};


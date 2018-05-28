#pragma once
#include <SFML/Window.hpp>
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
	//void initOpenGLContext();
	sf::Window window;
	std::shared_ptr<tetraRender::Scene> scene;
	sf::Clock clock;
};


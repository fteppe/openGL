#pragma once
#include <SFML/Window.hpp>
//
class WindowBuilder
{
public:
	WindowBuilder();
	~WindowBuilder();

	void draw();

private:
	//void initOpenGLContext();
	sf::Window window;
};


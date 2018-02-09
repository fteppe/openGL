#pragma once
#include <SFML/Window.hpp>
#include "Shader.h"
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


#pragma once

#include <tetraRender/Scene.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
//
class WindowBuilder
{
public:
	WindowBuilder();
	WindowBuilder(std::string scene);
	~WindowBuilder();

	void draw();

private:
	SDL_Window* window;
	SDL_GLContext gl_context;
	EventHandler handler;
	std::shared_ptr<tetraRender::Scene> scene;
};


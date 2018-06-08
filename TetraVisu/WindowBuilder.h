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
	glm::vec3 Vec3Input(glm::vec3 vec, std::string label);
	void MaterialUI(std::shared_ptr<tetraRender::Material> mat);
	//if a children is selected then a parent won't be (not sure if effective)
	void gameObjectTreeUI(tetraRender::GameObject* gameObject, int pos);
	void gameObjectEditUI(tetraRender::GameObject* gameObject);
	void parameterInput(tetraRender::ParameterContainer & param, tetraRender::Resource& resource);
	void gameObjectContext(tetraRender::GameObject* gameobject, int id);

	tetraRender::GameObject* selectedObject;
	SDL_Window* window;
	SDL_GLContext gl_context;
	EventHandler handler;
	std::shared_ptr<tetraRender::Scene> scene;
};


#pragma once

#include <tetraRender/Scene.h>
#include <SDL2/SDL.h>
#include "EventHandler.h"
#include "ResourcesLibrary.h"
#include "WaveFrontImporter.h"
//
class WindowBuilder
{
public:
	WindowBuilder();
	WindowBuilder(std::string scene);
	~WindowBuilder();

	void draw();
	void loadScene(std::string scene);
	static void MaterialUI(tetraRender::Material* mat, tetraRender::ResourceAtlas& atlas);
	//if a children is selected then a parent won't be (not sure if effective)
	static void textureUI(tetraRender::Texture* tex);
	static void shaderUI(tetraRender::Shader* shader);
	void gameObjectTreeUI(tetraRender::GameObject* gameObject, int pos);
	static void gameObjectEditUI(tetraRender::GameObject* gameObject, tetraRender::ResourceAtlas & atlas);
	static void parameterInput(tetraRender::ParameterContainer & param, tetraRender::Resource& resource);
	static std::shared_ptr<tetraRender::Shader> selectShader(tetraRender::ResourceAtlas& atlas);
	static std::shared_ptr<tetraRender::Texture> selectTexture(std::string channel, tetraRender::ResourceAtlas& atlas, std::shared_ptr<tetraRender::Texture> selectedTexture);
	static std::shared_ptr<tetraRender::Material> selectMaterial(tetraRender::ResourceAtlas& atlas, std::shared_ptr<tetraRender::Material> currentMat);
	void gameObjectContext(tetraRender::GameObject* gameobject, int id);
	void menu();

private:
	static glm::vec3 Vec3Input(glm::vec3 vec, std::string label);
	static std::string stringInput(std::string input, std::string label);

	WaveFrontImporter importer;
	std::string sceneName;
	tetraRender::GameObject* selectedObject;
	SDL_Window* window;
	SDL_GLContext gl_context;
	EventHandler handler;
	std::shared_ptr<tetraRender::Scene> scene;
	ResourcesLibrary library;
};


#pragma once
#include "Camera.h"
#include "Light.h"
#include "SceneLoader.h"
#include "FrameBuffer.h"
#include <vector>
#include <SFML/Window.hpp>
//#include "GameObject.h"
/*
This class contains objects, a light and a camera. So we should be able to render a scene.
*/
class Scene
{
	friend class EventHandler;
public:
	Scene(Camera cam);
	void animate(sf::Clock elapsed);
	~Scene();
	void setCamera(Camera camera);
	void renderScene();
	void load(std::string scene);

	Camera getCam() const;
	Light getLight() const;
	std::shared_ptr<Texture> getTexture(std::string tex) ; 
	float getElapsedTime() const;

	//TODO: remove it and replace by proper renderPass object
	unsigned renderPass;

private:
	void makeSkyBox();
	void setupPostProcessing();

	Camera cam;
	Light light;
	sf::Clock clock;
	std::vector<GameObject*> elements;
	GameObject* skybox;
	FrameBuffer* frame;
	SceneLoader loader;

	VBO_CONTAINER models;
	MAT_CONTAINER materials;
	SHADER_CONTAINER shaders;
	TEXTURE_CONTAINER textures;

};


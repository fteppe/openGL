#pragma once
#include "Camera.h"
#include "Light.h"
#include "SceneLoader.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "Common.h"
#include <vector>
#include <SFML/Window.hpp>

namespace tetraRender
{
	class RenderPass;

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
		/** This is the rendering pipeline and currently it is very generic which can be nice but it means some things are hard to do
		With a more rigid pipeline I can have more controle in each step. Since I currently have very few passes it makes sense.
		*/
		void renderScene();
		void load(std::string scene);

		Camera getCam() const;

		//These functions should change soon, I want lights with a potential camera in it, the camera will be the projection.
		//If there is a camera there should also be a renderPass. But I don't know if it should be part of the light ot not.
		Camera getShadowProj() const;
		Light getLight() const;
		std::shared_ptr<Texture> getTexture(std::string tex);
		float getElapsedTime() const;
		std::vector<GameObject *> getGameObjects();


	private:
		void makeSkyBox();
		void setupPostProcessing();

		Camera cam;
		//I want to have an array of lights, that can have a camera that does the projection shadow.
		//I don't want to deal with supporting omni lights that generate 3D shadows for now.
		Camera shadowProjection;
		Light light;
		sf::Clock clock;
		std::vector<GameObject*> gameObjects;
		GameObject* skybox;
		//SceneLoader loader;

		//These are stored in the order in which they must be done.
		std::vector<RenderPass*> renderPasses;
		//I want to seperate render passes with a specific purpose (geometry, deffered shading, post process) and the building of shadowmaps.
		VBO_CONTAINER models;
		MAT_CONTAINER materials;
		SHADER_CONTAINER shaders;
		//I want to separate the textures of a scene and the G-Buffer and shadowmaps.
		TEXTURE_CONTAINER textures;

	};


}
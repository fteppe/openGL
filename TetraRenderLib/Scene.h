#pragma once
#include "Camera.h"
#include "Light.h"
#include "SceneLoader.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
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
		void renderScene();
		void load(std::string scene);

		Camera getCam() const;
		Light getLight() const;
		std::shared_ptr<Texture> getTexture(std::string tex);
		float getElapsedTime() const;
		std::vector<GameObject *> getGameObjects();


	private:
		void makeSkyBox();
		void setupPostProcessing();

		Camera cam;
		//Camera shadowProjection;
		Light light;
		sf::Clock clock;
		std::vector<GameObject*> gameObjects;
		GameObject* skybox;
		SceneLoader loader;

		//These are stored in the order in which they must be done.
		std::vector<RenderPass*> renderPasses;

		VBO_CONTAINER models;
		MAT_CONTAINER materials;
		SHADER_CONTAINER shaders;
		TEXTURE_CONTAINER textures;

	};


}
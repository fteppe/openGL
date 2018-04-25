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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor.  Builds a scene with the basic elements that constitute any scene. This includes the rendering pipeline framebuffers.</summary>
		///
		/// <remarks>	Fteppe, 20/04/2018. </remarks>
		///
		/// <param name="cam">	The camera that willbe used to render the scene </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scene(Camera cam);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Animates the scene depending on the current time. THe current way it is built isn't use but might come
		/// 			back if I put animations back in. </summary>
		///
		/// <remarks>	To be deprecated </remarks>
		///
		/// <param name="elapsed">	The elapsed time since the creation of the scene </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void animate(sf::Clock elapsed);
		~Scene();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	changes the camera used to render the scene. </summary>
		///
		/// <remarks>	Fteppe, 20/04/2018. </remarks>
		///
		/// <param name="camera">	The camera. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void setCamera(Camera camera);
		/** This is the rendering pipeline and currently it is very generic which can be nice but it means some things are hard to do
		With a more rigid pipeline I can have more controle in each step. Since I currently have very few passes it makes sense.
		*/
		void renderScene();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads the given scene. It includes shader programs, materials, VBOs and solids.</summary>
		///
		/// <remarks>	Very basic and should at some point use the power of lbraries like ASSIMP to offer loading more complexe scene. The specific aren't defined yet. </remarks>
		///
		/// <param name="scene">	the JSON file used to load the scene </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void load(std::string scene);

		Camera getCam() const;
		std::vector<Light*> getLights(GameObject* root) const;

		//These functions should change soon, I want lights with a potential camera in it, the camera will be the projection.
		//If there is a camera there should also be a renderPass. But I don't know if it should be part of the light ot not.
		Camera getShadowProj() const;
		Light getLight() const;
		std::shared_ptr<Texture> getTexture(std::string tex);
		float getElapsedTime() const;
		std::vector<GameObject *> getGameObjects();


	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes sky box, will load the cube, create the shader and material and load the textures. The used skybox is currently hard coded. </summary>
		///
		/// <remarks>	Fteppe, 20/04/2018. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

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
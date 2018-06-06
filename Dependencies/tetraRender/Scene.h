#pragma once
#include "Camera.h"
#include "Light.h"
#include "SceneLoader.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "Common.h"
#include "RenderPipeline.h"
#include <vector>

namespace tetraRender
{
	class RenderPass;
	class RenderPipeline;
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

		void animate(float elapsed);
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
		void addGameObject(GameObject * obj);


		std::shared_ptr<Texture> getTexture(std::string tex);
		float getElapsedTime() const;
		GameObject* getGameObjects();
		
		std::vector<Light * > getLights();
		MAT_CONTAINER & getMaterials();
		RenderPipeline& getRenderPipeLine();

	private:
		std::vector<Light*> getLights(GameObject* root) const;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes sky box, will load the cube, create the shader and material and load the textures. The used skybox is currently hard coded. </summary>
		///
		/// <remarks>	Fteppe, 20/04/2018. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void makeSkyBox();
		
		/// <summary>
		/// Updates the shadow maps, it will take all the lights that project shadows and assign them a shadowmap.
		/// </summary>
		void updateShadowMaps();

		Camera cam;
		//I want to have an array of lights, that can have a camera that does the projection shadow.
		//I don't want to deal with supporting omni lights that generate 3D shadows for now.


		GameObject* gameObjects;
		//SceneLoader loader;

		//I want to seperate render passes with a specific purpose (geometry, deffered shading, post process) and the building of shadowmaps.
		// Now the rendering will hapen in this, so the scene is no longer responsible on the rendering.
		std::unique_ptr<tetraRender::RenderPipeline> renderPipeLine;
		VBO_CONTAINER models;
		MAT_CONTAINER materials;
		SHADER_CONTAINER shaders;
		//I want to separate the textures of a scene and the G-Buffer and shadowmaps.
		TEXTURE_CONTAINER textures;

	};


}
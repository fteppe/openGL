#pragma once
#include "FrameBuffer.h"
#include <map>
#include "Texture.h"
#include "Solid.h"
#include "RenderPass.h"
#include <glm/mat4x4.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////
// namespace: tetrRender
//
// summary:	the pipeline will handle all of the step in rendering. This is to avoid doing all this in the scene
// Also in the future it allows us to have inheritance in the pipeline to do rendering differently.
// For now the steps are all hardcoded in the engine without any scripting possible to change it.
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace tetraRender
{
	class RenderPass;
	class RenderPipeline
	{
	public:
		RenderPipeline(Scene & scene);
		~RenderPipeline();		
		/// <summary>
		/// Renders the scene, It does that by going through the whole pipeline, rendering lightmaps and depending on the tags of the objects it renders them or not.
		/// </summary>
		/// <param name="scene">The scene.</param>
		void renderScene(Scene& scene);
		
		/// <summary>
		/// Sets the shadow's PoV for a certain renderPass.
		/// </summary>
		/// <param name="PoV">The PoV of the light.</param>
		/// <param name="index">The index of the renderPass that will have it's Camera changed.</param>
		void setShadowPoV(Camera* PoV, int index);
		void setNumShadows(unsigned int numShadows);
		void update(Scene& scene);
		std::vector<std::unique_ptr<RenderPass>>& getShadowMapsPass();
		

	private:		
		/// <summary>
		/// Setups the post processing pipeline, creating the textures for the G-buffer and loading the 
		/// right shaders and materials.
		/// </summary>
		/// <param name="scene">The scene.</param>
		void setupPostProcessing(Scene & scene);		
		/// <summary>
		/// Setups the rendering passes and the shadowmap passes.
		/// </summary>
		void setupRenderPasses();

		/// <summary>	Contains all the textures of the different steps of rendering </summary>
		std::map<std::string, std::shared_ptr<Texture>> gBuffer;

		/// <summary>	The framebuffers of this pipeline, they are only the regular framebuffers that I seperated from the shadowmaps framebuffers. </summary>
		std::vector<std::unique_ptr<tetraRender:: RenderPass>> renderPasses;

		/// <summary>	The shadowmaps passes, they are seperated from the other render passes.
		/// THe renderpass contains the camera that is the point of view of the light</summary>
		std::vector<std::unique_ptr<tetraRender:: RenderPass>> shadowmapsPasses;
		std::vector<std::shared_ptr<Texture>> shadowMaps;

		/// <summary>	The render surfaces. For each post processing renderpasses we need a surface to render on.
		/// 			Each of those surfaces have a different shader that contains the post processing code, and a different material that defines  </summary>
		std::vector<std::unique_ptr<Solid>> renderSurfaces;		
		/// <summary>
		/// The number of shadwos that are currently necessary.
		/// </summary>
		unsigned int numShadows;
	};
}



#pragma once
#include "FrameBuffer.h"
#include <map>
#include "Texture.h"
#include "Solid.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// namespace: tetrRender
//
// summary:	the pipeline will handle all of the step in rendering. This is to avoid doing all this in the scene
// Also in the future it allows us to have inheritance in the pipeline to do rendering differently.
// For now the steps are all hardcoded in the engine without any scripting possible to change it.
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tetraRender
{
	class RenderPipeline
	{
	public:
		RenderPipeline();
		~RenderPipeline();

	private:

		void setupPostProcessing();
		void setupRenderPasses();

		/// <summary>	Contains all the textures of the different steps of rendering </summary>
		std::map<std::string, std::shared_ptr<Texture>> gBuffer;

		/// <summary>	The framebuffers of this pipeline, they are only the regular framebuffers that I seperated from the shadowmaps framebuffers. </summary>
		std::vector<std::unique_ptr<RenderPass>> renderPasses;

		std::vector<std::unique_ptr<RenderPass>> shadowmapsPasses;
		std::vector<std::shared_ptr<Texture>> shadowmaps;

		/// <summary>	The render surfaces. For each post processing renderpasses we need a surface to render on.
		/// 			Each of those surfaces have a different shader that contains the post processing code, and a different material that defines  </summary>
		std::vector<std::unique_ptr<Solid>> renderSurfaces;
	};
}



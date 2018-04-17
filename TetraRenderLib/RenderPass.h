#pragma once
#include <set>
#include "Common.h"
#include "FrameBuffer.h"
#include "Material.h"
#include "Camera.h"
#include "Texture.h"
#include "Scene.h"
/*
*This class aims at having all the informations to render a scene.
*It contains uniforms specific to this redner pass.
*A renderPass also has aframeBuffer so rendering is stored in a texture.
*/
namespace tetraRender
{
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();
		void setRenderOutput(FrameBuffer * output);
		void setCamera(Camera* camera);
		void setRenderTagsIncluded(std::vector<RenderTag> tags);
		void setRenderTagsExcluded(std::vector<RenderTag> tags);
		void renderScene(Scene & scene);
		void setTextures(std::map < std::string, std::shared_ptr<Texture>> texturesIn);
		void setMat(std::shared_ptr<Material> mat);

	private:

		bool isIntersectionEmpty(std::vector<RenderTag> renderPassTags, std::set<RenderTag> elementTags);

		//A render pass always needs a view point, in the case the renderPass is to get shadows the camera will be the shadow's viewpoint
		Camera* camera_ptr;
		//This determines where a RenderPass renders.
		//The renderPass is the sole owner of the frameBuffer is in in charge of deleting it.
		FrameBuffer* renderOutput;
		//This render pass will render only the game objects that have these tags 
		std::vector<RenderTag> renderTagsIncluded;
		//This renderPass will render all objects excluded the ones containing these tags.
		std::vector<RenderTag> renderTagsExcluded;
		//In the case we want to render the entire pass with a specific material, we use this. It can be interesting in the case we only need the depth component, then we only need a very simple material.
		std::shared_ptr<Material> optionalMaterial;
		//in case this renderpass needs specific textures that will be sent to all the program used in this render pass.
		//For exemple, when drawing a scene with shadows, some necessary textures are shadowmaps or rendering from other viewpoints for reflections.
		std::map<std::string, std::shared_ptr<Texture>> textures;

		//If a render tag of a gameobject is in both in excluded and included tags, this 
		//boolean determines whixh one is prioritary.
		bool inclusionIsPrioritary;

	};

}

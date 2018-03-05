#include "stdafx.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "Common.h"

//By default a renderPass renders to the screen.
RenderPass::RenderPass() : renderOutput(new FrameBuffer(SCREEN_FRAMEBUFFER))
{
}


RenderPass::~RenderPass()
{
	delete renderOutput;
	renderOutput = NULL;
}

void RenderPass::setRenderOutput(FrameBuffer * output)
{
	//Before we change the output, we delete the previous one to avoid hangin memory usage.
	delete renderOutput;
	renderOutput = output;
}

void RenderPass::setCamera(Camera camera)
{
	this->camera = camera;
}

void RenderPass::setRenderTagsIncluded(std::vector<RenderTag> tags)
{
	this->renderTagsIncluded = tags;
}

void RenderPass::renderScene(Scene & scene)
{
	renderOutput->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO: we must be able to go further than that in the future.
	std::vector<GameObject*> objects = scene.getGameObjects();
	Shader * shader_ptr;
	//we iterate through all our objects, and we see which one we render.
	for (GameObject* go : objects)
	{
		//If our object has one tag part of the included tags to be rendered then we do.
		if (!isIntersectionEmpty(this->renderTagsIncluded, go->getRenderTags()))
		{
			go->draw(scene);
			//shader_ptr = go->getMaterial()->getShaderProgram().get();
		}
	}
	//renderOutput->renderToScreen();
}

void RenderPass::setTextures(std::map<std::string, std::shared_ptr<Texture>> texturesIn)
{
	textures = texturesIn;
}

bool RenderPass::isIntersectionEmpty(std::vector<RenderTag> renderPassTags, std::set<RenderTag> elementTags)
{
	bool isIntersectionEmpty = true;
	//we look for each tags of this render pass if it exists in the gameobject's tags set.
	//if one in common is found we break and return true.
	//else we retrun false.
	for (auto tag : renderPassTags)
	{
		if (elementTags.find(tag) != elementTags.end())
		{
			isIntersectionEmpty = false;
			break;
		}
	}

	return isIntersectionEmpty;
}

#include "stdafx.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "Common.h"

using namespace tetraRender;

//By default a renderPass renders to the screen.
RenderPass::RenderPass() : renderOutput(new FrameBuffer(SCREEN_FRAMEBUFFER))
{
}


RenderPass::~RenderPass()
{
}

void RenderPass::setRenderOutput(std::shared_ptr<FrameBuffer> output)
{
	//Before we change the output, we delete the previous one to avoid hangin memory usage.
	renderOutput = output;
}

void RenderPass::setCamera(Camera* camera)
{
	this->camera_ptr = camera;
}

void RenderPass::setRenderTagsIncluded(std::vector<RenderTag> tags)
{
	this->renderTagsIncluded = tags;
}

void RenderPass::renderScene(tetraRender::Scene & scene)
{
	//since this renderpass has a different camera from the scene's main camera we change it.
	Camera tempCamera = scene.getCam();
	if (camera_ptr != NULL)
	{
		scene.setCamera(*camera_ptr);
	}
	renderOutput->bind();
	//TODO: we must be able to go further than that in the future.
	std::vector<GameObject*> gos = this->getObjectsToDraw(scene.getGameObjects());
	//we iterate through all our objects, and we see which one we render.

		//If our object has one tag part of the included tags to be rendered then we do.
	for (auto go : gos)
	{
		if (optionalMaterial)
		{
			go->draw(scene, this->optionalMaterial);
		}
		else
		{
			go->draw(scene);
		}
	}

	
	scene.setCamera(tempCamera);
	//renderOutput->renderToScreen();
}

void RenderPass::setTextures(std::map<std::string, std::shared_ptr<Texture>> texturesIn)
{
	textures = texturesIn;
}

void tetraRender::RenderPass::setMat(std::shared_ptr<Material> mat)
{
	optionalMaterial = mat;
}

std::shared_ptr<Material> tetraRender::RenderPass::getMaterial()
{
	return optionalMaterial;
}

std::vector<GameObject*> tetraRender::RenderPass::getObjectsToDraw(GameObject * gameObject)
{
	//TODO: fix all this algorythm that is wrong.
	std::vector<GameObject*> returnVal;

	for (auto goChild : gameObject->getChildren())
	{
		auto childrenObjs = getObjectsToDraw(goChild);
		returnVal.insert(returnVal.end(), childrenObjs.begin(), childrenObjs.end());
	}
	//we check if the game object should be rendered.
	if (!isIntersectionEmpty(this->renderTagsIncluded, gameObject->getRenderTags()))
	{
			returnVal.push_back(gameObject);
	}
	
	return returnVal;
}

Camera * tetraRender::RenderPass::getCamera()
{
	return camera_ptr;
}

FrameBuffer & tetraRender::RenderPass::getFrameBuffer()
{
	return *renderOutput;
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

#include "stdafx.h"
#include "RenderPass.h"


RenderPass::RenderPass()
{
}


RenderPass::~RenderPass()
{
}

void RenderPass::setRenderOutput(FrameBuffer * output)
{
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

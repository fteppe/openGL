#include "stdafx.h"
#include "ResourcesLibrary.h"
#include "WindowBuilder.h"
#include "imgui.h"

ResourcesLibrary::ResourcesLibrary()
{
}


ResourcesLibrary::ResourcesLibrary(tetraRender::ResourceAtlas * resourcesIn)
{
	resources = resourcesIn;
}

void ResourcesLibrary::display()
{
	ImGui::Begin("resources");
	for (auto tex : resources->getTextures())
	{
		ImGui::Text(tex.second->getName().c_str());
		WindowBuilder::textureUI(tex.second.get());
	}
	ImGui::End();
}

ResourcesLibrary::~ResourcesLibrary()
{
}

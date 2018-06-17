#include "stdafx.h"
#include "ResourcesLibrary.h"
#include "WindowBuilder.h"
#include "imgui.h"

ResourcesLibrary::ResourcesLibrary()
{
	matEditorOpen = -1;
	textureEditorOpen = -1;
	shaderEditorOpen = -1;
}


ResourcesLibrary::ResourcesLibrary(tetraRender::ResourceAtlas * resourcesIn) : ResourcesLibrary()
{

	resources = resourcesIn;
}

void ResourcesLibrary::display()
{
	ImGui::Begin("resources");
	if (ImGui::TreeNode("materials"))
	{
		int i = 0;
		for (auto mat : resources->getMaterials())
		{
			ImGui::Button(mat.second->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				matEditorOpen = i;
			}
			if (matEditorOpen == i)
			{
				ImGui::Begin("mat");
				WindowBuilder::MaterialUI(mat.second.get(), *this->resources);
				if (ImGui::Button("OK", ImVec2(120, 0))) { matEditorOpen = -1; }
				ImGui::End();
			}
			i++;
		}
		ImGui::Separator();
		ImGui::Button("add material");
		if (ImGui::IsItemClicked())
		{
			resources->addMaterial(std::shared_ptr<tetraRender::Material>(new tetraRender::Material));
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("textures"))
	{
		int i = 0;
		for (auto tex : resources->getTextures())
		{
			
			ImGui::Button(tex.second->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				textureEditorOpen = i;
				//ImGui::OpenPopup(tex.second->getName().c_str());
			}
			if (i == textureEditorOpen/*ImGui::BeginPopupModal(tex.second->getName().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)*/)
			{
				ImGui::Begin("texture");
				WindowBuilder::textureUI(tex.second.get());
				ImGui::Button("OK", ImVec2(120, 0));
				if (ImGui::IsItemClicked()) { textureEditorOpen = -1; }
				ImGui::End();
			}
			i++;
		}
		
		ImGui::Button("add texture");
		if (ImGui::IsItemClicked())
		{
			resources->addTexture(std::shared_ptr<tetraRender::Texture>(new tetraRender::Texture));
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("shaders"))
	{
		int i = 0;
		for (auto shader : resources->getShaders())
		{
			ImGui::Button(shader.second->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				shaderEditorOpen = i;
				//ImGui::OpenPopup(tex.second->getName().c_str());
			}
			if (i == shaderEditorOpen/*ImGui::BeginPopupModal(tex.second->getName().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)*/)
			{
				ImGui::Begin("Shader");
				WindowBuilder::shaderUI(shader.second.get());
				ImGui::Button("OK", ImVec2(120, 0));
				if (ImGui::IsItemClicked()) { shaderEditorOpen = -1; }
				ImGui::End();
			}
			i++;
			
		}
	}

	ImGui::End();
}

ResourcesLibrary::~ResourcesLibrary()
{
}

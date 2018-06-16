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
	if (ImGui::TreeNode("materials"))
	{
		for (auto mat : resources->getMaterials())
		{
			ImGui::Button(mat.second->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				ImGui::OpenPopup(mat.second->getName().c_str());
			}
			if (ImGui::BeginPopupModal(mat.second->getName().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				WindowBuilder::MaterialUI(mat.second.get(), *this->resources);
				if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("textures"))
	{
		for (auto tex : resources->getTextures())
		{
			ImGui::Button(tex.second->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				ImGui::OpenPopup(tex.second->getName().c_str());
			}
			if (ImGui::BeginPopupModal(tex.second->getName().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				WindowBuilder::textureUI(tex.second.get());
				if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

ResourcesLibrary::~ResourcesLibrary()
{
}

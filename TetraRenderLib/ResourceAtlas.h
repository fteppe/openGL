#pragma once
#include <map>
#include <memory>
#include "Material.h"
#include "GameObject.h"

namespace tetraRender
{
	template<class T> 
	using AtlasContainer = std::map<std::string, std::shared_ptr<T>>;
	//typedef std::map<std::string, std::shared_ptr<typename T>> resourceContainer<typename T>;
	class ResourceAtlas
	{
	public:
		ResourceAtlas();
		~ResourceAtlas();

	private:
		AtlasContainer<Texture> textures;
		AtlasContainer<Shader> shaders;
		AtlasContainer<Material> materials;
		GameObject* root;
	};
}



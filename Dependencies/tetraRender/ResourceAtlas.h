#pragma once
#include <map>
#include <memory>
#include "Material.h"
#include "GameObject.h"

namespace tetraRender
{
	template<class T> 
	using AtlasContainer = std::map<std::string, std::shared_ptr<T>>;
	using MeshContainer = std::map < std::string, AtlasContainer<Mesh>>;
	//typedef std::map<std::string, std::shared_ptr<typename T>> resourceContainer<typename T>;
	class ResourceAtlas
	{
	public:
		ResourceAtlas();
		~ResourceAtlas();
		std::string addResource(AtlasContainer<Resource> container, std::shared_ptr<Resource> resource);


	private:
		AtlasContainer<Texture> textures;
		AtlasContainer<Shader> shaders;
		AtlasContainer<Material> materials;
		MeshContainer meshes;
		GameObject* root;
	};

}



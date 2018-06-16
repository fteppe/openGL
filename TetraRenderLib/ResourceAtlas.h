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
		std::string addTexture(std::shared_ptr<Texture> texture);
		std::shared_ptr<Texture> getTexture(std::string texName);
		std::string addShader(std::shared_ptr<Shader> shader);
		std::shared_ptr<Shader> getShader(std::string shaderName);
		std::string addMaterial(std::shared_ptr<Material> mat);
		std::shared_ptr<Material> getMaterial(std::string materialName);
		const MeshContainer& getMeshes();
		const AtlasContainer<Texture>& getTextures();
		const AtlasContainer<Shader>& getShaders();
		void addMesh( std::shared_ptr<Mesh> mesh);
		std::string getUniqueGameObjectName(std::string gameObjectName);


	private:
		AtlasContainer<Texture> textures;
		AtlasContainer<Shader> shaders;
		AtlasContainer<Material> materials;
		MeshContainer meshes;
		std::set < std::string> gameObjectNames;
		std::set<std::string> loadedFiles;
		GameObject* root;
	};

}



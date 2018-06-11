#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "GameObject.h"
#include "ResourceAtlas.h"
#include "Common.h"


namespace tetraRender
{


	class SceneLoader
	{
	public:
		SceneLoader();
		~SceneLoader();
		void setSceneToLoad(std::string file);
		void  loadModels(ResourceAtlas& atlas);
		void loadTextures(ResourceAtlas& atlas);
		void loadShaders(ResourceAtlas& atlas);
		void loadMaterials(ResourceAtlas& atlas);
		std::vector<GameObject *> loadGameObjects(ResourceAtlas& atlas);
		GameObject* loadSingleGameObject(ResourceAtlas& atlas, rapidjson::Value& go);
		Solid* loadSolid(ResourceAtlas& atlas, rapidjson::Value& go);
		Light* loadLight(rapidjson::Value& go);
		void setResourceParam(Resource& resource, rapidjson::Value& resourceJSON);
	private:
		Texture * loadTexture(rapidjson::Value& texture);
		rapidjson::Document doc;

	};


}
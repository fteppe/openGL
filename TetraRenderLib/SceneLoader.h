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
#include "Common.h"


namespace tetraRender
{


	class SceneLoader
	{
	public:
		SceneLoader();
		~SceneLoader();
		void setSceneToLoad(std::string file);
		VBO_CONTAINER  loadModels();
		TEXTURE_CONTAINER loadTextures();
		SHADER_CONTAINER loadShaders();
		MAT_CONTAINER loadMaterials(TEXTURE_CONTAINER& textures, SHADER_CONTAINER& shaders);
		std::vector<GameObject *> loadGameObjects(MAT_CONTAINER& mats, VBO_CONTAINER& objects);
		GameObject* loadSingleGameObject(MAT_CONTAINER& mats, VBO_CONTAINER& objects, rapidjson::Value& go);
		Solid* loadSolid(MAT_CONTAINER& mats, VBO_CONTAINER& objects, rapidjson::Value& go);
		Light* loadLight(rapidjson::Value& go);
	private:
		Texture * loadTexture(rapidjson::Value& texture);
		rapidjson::Document doc;

	};


}
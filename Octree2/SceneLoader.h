#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>
#include "VertexBufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "GameObject.h"


#define VBO_CONTAINER std::map<std::string, std::map<std::string, std::shared_ptr<VertexBufferObject>>>
#define MAT_CONTAINER std::map<std::string, std::shared_ptr<Material>>
#define TEXTURE_CONTAINER std::map<std::string, std::shared_ptr<Texture>>
#define SHADER_CONTAINER std::map<std::string, std::shared_ptr<Shader>>

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
private:
	Texture * loadTexture(std::string texturePath);
	rapidjson::Document doc;

};


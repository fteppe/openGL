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

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();
	void setSceneToLoad(std::string file);
	std::vector<VertexBufferObject*>  loadModels();
	std::map<std::string,Texture*> loadTextures();
	std::map<std::string, Shader*> loadShaders();
	std::map<std::string, Material*> loadMaterials(std::map<std::string, Texture*> textures, std::map<std::string, Shader*> shaders);
	std::vector<GameObject *> loadGameObjects(std::map<std::string, Material *> mats, std::map<std::string, std::map<std::string, VertexBufferObject*>> objects);

private:
	rapidjson::Document doc;

};


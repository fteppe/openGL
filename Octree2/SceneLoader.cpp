#include "stdafx.h"
#include "SceneLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "waveFrontLoader.h"
#include "ShaderPBR.h"

SceneLoader::SceneLoader()
{
	//We first test out the use of rapidJSON
	//loading the file
	
	//loadModels();

}


SceneLoader::~SceneLoader()
{
}

void SceneLoader::setSceneToLoad(std::string file)
{
	std::string scene = file;
	std::ifstream sceneSource(scene);
	std::string json = std::string((std::istreambuf_iterator<char>(sceneSource)), std::istreambuf_iterator<char>());

	//

	doc.Parse(json.c_str());
	loadModels();
	auto text = loadTextures();
	auto shaders = loadShaders();
	loadMaterials(text, shaders);
}

std::vector<VertexBufferObject*> SceneLoader::loadModels()
{
	//rapidjson::Value& objects = d["models"];
	std::vector<VertexBufferObject*> objects;
	WaveFrontLoader loader;
	rapidjson::Value::MemberIterator iterator = doc.FindMember("models");
	//assert(iterator != d.End());
	assert(iterator->value.IsArray());
	assert(iterator->value[0].IsString());
	rapidjson::Value& models = iterator->value;
	for (auto i = models.Begin(); i != models.End(); i++)
	{
		loader.loadVertexObjectVectorFromFile(i->GetString(), objects);
	}
	return objects;
}

std::map<std::string,Texture*> SceneLoader::loadTextures()
{
	std::map<std::string,Texture*> textures;
	rapidjson::Value::MemberIterator iterator = doc.FindMember("textures");
	//assert(iterator != d.End());
	assert(iterator->value.IsArray());
	assert(iterator->value[0].IsString());
	rapidjson::Value& texs = iterator->value;
	for (auto i = texs.Begin(); i != texs.End(); i++)
	{
		textures[i->GetString()] = new Texture();
		textures[i->GetString()]->loadTexture(i->GetString());
	}
	return textures;
}

std::map<std::string, Shader*> SceneLoader::loadShaders()
{

	std::map<std::string, Shader*> shaders;
	rapidjson::Value& shadersArray = doc["shaders"];
	assert(shadersArray.IsArray());
	for (unsigned int i = 0; i < shadersArray.Size(); i++)
	{
		std::vector < std::string > vertexShaderFiles;
		std::vector < std::string > fragmentShaderFiles;
		std::string shaderName = shadersArray[i]["name"].GetString();
		std::string shaderType = shadersArray[i]["type"].GetString();

		//We get the vertex shader files from here
		auto itvert = shadersArray[i].FindMember("vertex");
		assert(itvert->value.IsArray());
		rapidjson::Value& vertexShaders = itvert->value;
		for (unsigned int j = 0; j < vertexShaders.Size(); j++)
		{
			vertexShaderFiles.push_back(vertexShaders[i].GetString());
		}

		//fragment shader files
		auto itfrag = shadersArray[i].FindMember("fragment");
		assert(itvert->value.IsArray());
		rapidjson::Value& framgentShaders = itvert->value;
		for (unsigned int j = 0; j < vertexShaders.Size(); j++)
		{
			fragmentShaderFiles.push_back(framgentShaders[i].GetString());
		}

		//if we have a PBR shader
		if (shaderType == "PBR")
		{
			shaders[shaderName] = new ShaderPBR(vertexShaderFiles, fragmentShaderFiles);
		}
	}

	return shaders;
}

std::map<std::string, Material*> SceneLoader::loadMaterials(std::map<std::string, Texture*> textures, std::map<std::string, Shader*> shaders)
{
	std::map<std::string, Material*> materials;

	rapidjson::Value& mats = doc["materials"];
	assert(mats.IsArray());
	for (unsigned int i = 0; i < mats.Size(); i++)
	{
		std::string matName = mats[i]["name"].GetString();
		std::map<std::string, std::string> channels;
		//we iterate accross the channels to find them all
		for (rapidjson::Value::MemberIterator j = mats[i]["channels"].MemberBegin(); j != mats[i]["channels"].MemberEnd(); j++)
		{
			channels[j->name.GetString()] = j->value.GetString();
		}
	}

	return materials;
}

std::vector<GameObject*> SceneLoader::loadGameObjects(std::map<std::string, Material*> mats, std::map<std::string, std::map<std::string, VertexBufferObject*>> objects)
{

	return std::vector<GameObject*>();
}


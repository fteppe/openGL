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

	doc.Parse(json.c_str());

}

VBO_CONTAINER SceneLoader::loadModels()
{
	//rapidjson::Value& objects = d["models"];
	std::map<std::string, std::map<std::string, std::shared_ptr<VertexBufferObject>>> objects;
	WaveFrontLoader loader;
	rapidjson::Value::MemberIterator iterator = doc.FindMember("models");
	//assert(iterator != d.End());
	assert(iterator->value.IsArray());
	assert(iterator->value[0].IsString());
	rapidjson::Value& models = iterator->value;
	std::vector<VertexBufferObject*> loaded;
	for (auto i = models.Begin(); i != models.End(); i++)
	{
		loader.loadVertexObjectVectorFromFile(i->GetString(), loaded);
	}
	for (auto obj : loaded)
	{
		std::pair<std::string, std::string> path = obj->getFilePath();
		objects[path.first][path.second] = std::shared_ptr<VertexBufferObject>(obj);
	}
	return objects;
}

TEXTURE_CONTAINER SceneLoader::loadTextures()
{
	TEXTURE_CONTAINER textures;
	rapidjson::Value::MemberIterator iterator = doc.FindMember("textures");
	rapidjson::Value& texs = doc["textures"];
	//assert(iterator != d.End());
	assert(texs.IsArray());

	for (unsigned int i = 0; i < texs.Size(); i++)
	{
		std::string texName = texs[i].GetString();
		textures[texName] = std::shared_ptr<Texture> (new Texture());
		textures[texName]->loadTexture(texName);
	}
	return textures;
}

SHADER_CONTAINER SceneLoader::loadShaders()
{

	SHADER_CONTAINER shaders;
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
			vertexShaderFiles.push_back(vertexShaders[j].GetString());
		}

		//fragment shader files
		auto itfrag = shadersArray[i].FindMember("fragment");
		assert(itfrag->value.IsArray());
		rapidjson::Value& framgentShaders = itfrag->value;
		for (unsigned int j = 0; j < framgentShaders.Size(); j++)
		{
			fragmentShaderFiles.push_back(framgentShaders[j].GetString());
		}

		//if we have a PBR shader
		if (shaderType == "shaderPBR")
		{
			shaders[shaderName] = std::shared_ptr<Shader> (new ShaderPBR(vertexShaderFiles, fragmentShaderFiles));
		}
	}

	return shaders;
}

MAT_CONTAINER SceneLoader::loadMaterials(TEXTURE_CONTAINER textures, SHADER_CONTAINER shaders)
{
	MAT_CONTAINER materials;

	rapidjson::Value& mats = doc["materials"];
	assert(mats.IsArray());
	for (unsigned int i = 0; i < mats.Size(); i++)
	{
		std::string matName = mats[i]["name"].GetString();
		std::string shaderName = mats[i]["shader"].GetString();
		std::map<std::string, std::string> channels;
		//we iterate accross the channels to find them all
		for (rapidjson::Value::MemberIterator j = mats[i]["channels"].MemberBegin(); j != mats[i]["channels"].MemberEnd(); j++)
		{
			channels[j->name.GetString()] = j->value.GetString();
		}

		materials[matName] = std::shared_ptr<Material>(new Material(shaders[shaderName].get()));
		for (auto tex : channels)
		{
			materials[matName]->setChannel(textures[tex.second].get(), tex.first);
		}
	}

	return materials;
}

std::vector<GameObject*> SceneLoader::loadGameObjects(MAT_CONTAINER mats, VBO_CONTAINER objects)
{
	std::vector<GameObject * > gameObjects;
	rapidjson::Value& gos = doc["gameObjects"];
	assert(gos.IsArray());

	for (unsigned int i = 0; i < gos.Size(); i++)
	{
		std::string type = gos[i]["type"].GetString();
		//we check what kind of gameObject this is
		if (type == "solid")
		{
			std::string mat = gos[i]["material"].GetString();
			std::pair<std::string, std::string> filePath;
			filePath.first = gos[i]["model"][0].GetString();
			filePath.second = gos[i]["model"][1].GetString();

			std::shared_ptr<VertexBufferObject> VBO = objects[filePath.first][filePath.second];
			gameObjects.push_back(new Solid(VBO));
			Solid* loadedItem = ((Solid *)gameObjects.back());
			loadedItem->setMaterial(std::shared_ptr<Material>(mats[mat]));
			//Since it was loaded as a game object we give it the tag.
			loadedItem->addTag(WORLD_OBJECT);
		}
	}
	return gameObjects;
}


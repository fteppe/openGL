#include "stdafx.h"
#include "SceneLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "waveFrontLoader.h"
#include "ShaderPBR.h"

using namespace tetraRender;

SceneLoader::SceneLoader()
{

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
	std::cout<<__FILE__<<" "<<__LINE__<<" loading models"<<std::endl;
	//rapidjson::Value& objects = d["models"];
	VBO_CONTAINER objects;
	WaveFrontLoader loader;
	//assert(doc.IsObject());
	rapidjson::Value& models = doc["models"];
	std::vector<VertexBufferObject*> loaded;
	for (unsigned int  i = 0; i < models.Size(); i++)
	{
		loader.loadVertexObjectVectorFromFile(models[i].GetString(), loaded);
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
	std::cout<<__FILE__<<" "<<__LINE__<<" loading textures"<<std::endl;
	TEXTURE_CONTAINER textures;
	rapidjson::Value::MemberIterator iterator = doc.FindMember("textures");
	rapidjson::Value& texs = doc["textures"];
	//assert(iterator != d.End());
	assert(texs.IsArray());

	for (unsigned int i = 0; i < texs.Size(); i++)
	{
		std::string texPath = texs[i].GetString();
		textures[texPath] = std::shared_ptr<Texture>(loadTexture(texPath));
	}
	return textures;
}

SHADER_CONTAINER SceneLoader::loadShaders()
{
	std::cout<<__FILE__<<" "<<__LINE__<<" loading shaders"<<std::endl;
	SHADER_CONTAINER shaders;
	assert(doc.IsObject());
	rapidjson::Value& shadersArray = doc["shaders"];
	//assert(shadersArray.IsArray());
	for (unsigned int i = 0; i < shadersArray.Size(); i++)
	{
		std::vector < std::string > vertexShaderFiles;
		std::vector < std::string > fragmentShaderFiles;
		std::string shaderName = shadersArray[i]["name"].GetString();
		std::string shaderType = shadersArray[i]["type"].GetString();

		//We get the vertex shader files from here
		auto itvert = shadersArray[i].FindMember("vertex");
		//assert(itvert->value.IsArray());
		rapidjson::Value& vertexShaders = itvert->value;
		for (unsigned int j = 0; j < vertexShaders.Size(); j++)
		{
			vertexShaderFiles.push_back(vertexShaders[j].GetString());
		}

		//fragment shader files
		auto itfrag = shadersArray[i].FindMember("fragment");
		//assert(itfrag->value.IsArray());
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

MAT_CONTAINER SceneLoader::loadMaterials(TEXTURE_CONTAINER& textures, SHADER_CONTAINER& shaders)
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

		materials[matName] = std::shared_ptr<Material>(new Material(shaders[shaderName]));
		for (auto tex : channels)
		{
			std::string texPath = tex.second;
			//In the case the texture wasn't loaded. We load it.
			if (textures.find(texPath) == textures.end())
			{
				textures[texPath] = std::shared_ptr<Texture>(loadTexture(texPath));
				
			}
			materials[matName]->setChannel(textures[texPath], tex.first);
		}
	}

	return materials;
}

std::vector<GameObject*> SceneLoader::loadGameObjects(MAT_CONTAINER& mats, VBO_CONTAINER& objects)
{
	std::vector<GameObject * > gameObjects;
	rapidjson::Value& gos = doc["gameObjects"];
	assert(gos.IsArray());

	for (unsigned int i = 0; i < gos.Size(); i++)
	{
		GameObject* loadedGo = NULL;
		rapidjson::Value& go = gos[i];
		std::string type = go["type"].GetString();
		//we check what kind of gameObject this is
		loadedGo = loadSingleGameObject(mats, objects, go);

		if (loadedGo != NULL)
		{
			gameObjects.push_back(loadedGo);
		}
	}
	return gameObjects;
}

GameObject* tetraRender::SceneLoader::loadSingleGameObject(MAT_CONTAINER & mats, VBO_CONTAINER & objects, rapidjson::Value& go)
{
	GameObject* loadedGo = NULL;
	std::string type = go["type"].GetString();
	//we check what kind of gameObject this is
	if (type == "solid")
	{

		std::string mat = go["material"].GetString();
		std::pair<std::string, std::string> filePath;
		filePath.first = go["model"][0].GetString();
		filePath.second = go["model"][1].GetString();

		std::shared_ptr<VertexBufferObject> VBO;// = objects[filePath.first][filePath.second];
												//In the case the object doesn't seem to exist.
		if (objects.find(filePath.first) == objects.end())
		{
			WaveFrontLoader loader;
			std::vector<VertexBufferObject*> objectsToLoad;
			loader.loadVertexObjectVectorFromFile(filePath.first, objectsToLoad);

			for (auto vbo : objectsToLoad)
			{
				objects[filePath.first][vbo->getFilePath().second] = std::shared_ptr<VertexBufferObject>(vbo);
				if (vbo->getFilePath().second == filePath.second)
				{
					VBO = objects[filePath.first][vbo->getFilePath().second];
				}
			}

		}
		else
		{
			VBO = objects[filePath.first][filePath.second];
		}

		//If we found the object we add it to our scene, but if we don't we just output an error.
		if (VBO != NULL)
		{
			loadedGo = new Solid(VBO);

			Solid* loadedItem = ((Solid *)loadedGo);
			loadedItem->setMaterial(std::shared_ptr<Material>(mats[mat]));
			//Since it was loaded as a game object we give it the tag.
			loadedItem->addTag(WORLD_OBJECT);
		}
		else
		{
			std::cout << "ERROR " + filePath.first + " " + filePath.second + " doesn't seem to exist" << std::endl;
		}
	}

	if (go.HasMember("children"))
	{
		rapidjson::Value& children = go["children"];
		//If we have children to load.
		if (children.IsArray())
		{
			for (rapidjson::Value& child : children.GetArray())
			{
				GameObject* childptr = loadSingleGameObject(mats, objects, child);
				if (childptr != NULL && loadedGo != NULL)
				{
					childptr->setParent(loadedGo);
					loadedGo->addChild(childptr);
				}
			}
		}
	}

	if (go.HasMember("pos"))
	{
		glm::vec3 pos(go["pos"]["x"].GetFloat(), go["pos"]["y"].GetFloat(), go["pos"]["z"].GetFloat());
		if (loadedGo != NULL)
		{
			loadedGo->setPos(pos);
		}
	}


	return loadedGo;
}

Texture * SceneLoader::loadTexture(std::string texturePath)
{
	std::string texName = texturePath;
	Texture *tex = (new Texture());
	tex->loadTexture(texName);
	return tex;
}


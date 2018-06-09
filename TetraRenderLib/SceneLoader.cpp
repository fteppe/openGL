#include "stdafx.h"
#include "SceneLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "CubeMap.h"
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
	if (doc.HasParseError())
	{
		std::cout << "PARSE ERROR \n";
	}

}

VBO_CONTAINER SceneLoader::loadModels()
{
	std::cout<<__FILE__<<" "<<__LINE__<<" loading models"<<std::endl;
	//rapidjson::Value& objects = d["models"];
	VBO_CONTAINER objects;
	WaveFrontLoader loader;
	//assert(doc.IsObject());
	rapidjson::Value& models = doc["models"];
	std::vector<Mesh*> loaded;
	for (unsigned int  i = 0; i < models.Size(); i++)
	{
		loader.loadVertexObjectVectorFromFile(models[i].GetString(), loaded);
	}
	for (auto obj : loaded)
	{
		std::pair<std::string, std::string> path = obj->getFilePath();
		objects[path.first][path.second] = std::shared_ptr<Mesh>(obj);
	}
	return objects;
}

TEXTURE_CONTAINER SceneLoader::loadTextures()
{
	std::cout<<__FILE__<<" "<<__LINE__<<" loading textures"<<std::endl;
	TEXTURE_CONTAINER textures;
	rapidjson::Value& texs = doc["textures"];
	//assert(iterator != d.End());
	assert(texs.IsArray());

	for (unsigned int i = 0; i < texs.Size(); i++)
	{
		std::shared_ptr<Texture> tex( loadTexture(texs[i]));
		textures[tex->getName()] = tex;
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
			auto newShader = std::shared_ptr<Shader>(new ShaderPBR(vertexShaderFiles, fragmentShaderFiles));
			newShader->setName(shaderName);
			newShader->getParameters().set("type", std::string(shaderType));
			shaders[shaderName] = newShader;
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
		if (shaders.find(shaderName) != shaders.end())
		{
			materials[matName] = std::shared_ptr<Material>(new Material(shaders[shaderName]));
			this->setResourceParam(*materials[matName], mats[i]);
			materials[matName]->setName(matName);
			for (rapidjson::Value::MemberIterator j = mats[i]["channels"].MemberBegin(); j != mats[i]["channels"].MemberEnd(); j++)
			{
				std::string channelName = j->name.GetString();
				std::shared_ptr<Texture> texture;
				if (j->value.IsString())
				{
					std::string texName = j->value.GetString();
					if (textures.find(texName) != textures.end())
					{
						texture = textures.find(texName)->second;
					}
					else
					{
						texture = std::shared_ptr<Texture>(loadTexture(j->value));
					}
				}
				else
				{
					texture = std::shared_ptr<Texture>(loadTexture(j->value));
				}
				
				//We only add the new texture if it doesn't already exists
				if (textures.find(texture->getName()) == textures.end())
				{
					textures[texture->getName()] = texture;
				}

				materials[matName]->setChannel(texture, channelName);

			}
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
		//we check what kind of gameObject this is
		loadedGo = loadSingleGameObject(mats, objects, go);

		if (loadedGo != NULL)
		{
			//gameObjects.push_back(loadedGo);
			gameObjects.push_back(loadedGo);
		}
	}
	return gameObjects;
}

GameObject* tetraRender::SceneLoader::loadSingleGameObject(MAT_CONTAINER & mats, VBO_CONTAINER & objects, rapidjson::Value& go)
{
	bool loadingWorked = true;
	GameObject* loadedGo = NULL;
	if (go["type"].IsString())
	{
		std::string type = go["type"].GetString();
		//we check what kind of gameObject this is
		if (type == "solid")
		{
			loadedGo = loadSolid(mats, objects, go);
			if (loadedGo == NULL)
			{
				loadingWorked = false;
			}
		}
		else if (type == "light")
		{
			//A bit early but it will do for now.
			loadedGo = loadLight(go);
			if (loadedGo == NULL)
			{
				loadingWorked = false;
			}
		}
	}
	else if(go["type"].IsNull())
	{
		loadedGo = new GameObject();
	}
	if(loadedGo != NULL)
	{
		setResourceParam(*loadedGo, go);
		loadedGo->update();
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
	}




	return loadedGo;
}

Solid * tetraRender::SceneLoader::loadSolid(MAT_CONTAINER & mats, VBO_CONTAINER & objects, rapidjson::Value & go)
{
	Solid * loadedGo = NULL;
	std::string mat = go["material"].GetString();
	std::pair<std::string, std::string> filePath;
	filePath.first = go["model"][0].GetString();
	filePath.second = go["model"][1].GetString();

	std::shared_ptr<Mesh> VBO;// = objects[filePath.first][filePath.second];
							  //In the case the object doesn't seem to exist.
	if (objects.find(filePath.first) == objects.end())
	{
		WaveFrontLoader loader;
		std::vector<Mesh*> objectsToLoad;
		loader.loadVertexObjectVectorFromFile(filePath.first, objectsToLoad);

		for (auto vbo : objectsToLoad)
		{
			objects[filePath.first][vbo->getFilePath().second] = std::shared_ptr<Mesh>(vbo);
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
		if (mats.find(mat) != mats.end())
		{
			loadedItem->setMaterial(std::shared_ptr<Material>(mats[mat]));
			//Since it was loaded as a game object we give it the tag.
			loadedItem->addTag(WORLD_OBJECT);
			loadedItem->setName(filePath.first + "::" + filePath.second);
		}
		else
		{
			delete loadedItem;
			loadedGo = NULL;
		}

		
	}
	else
	{
		std::cout << "ERROR " + filePath.first + " " + filePath.second + " doesn't seem to exist" << std::endl;
	}

	return loadedGo;
}

Light * tetraRender::SceneLoader::loadLight(rapidjson::Value & go)
{
	Light * light = new Light();
	light->getParameters().set(Light::intensity,go["intensity"].GetFloat());
	if (go.HasMember("color"))
	{
		glm::vec3 col = (glm::vec3(go["color"][0].GetFloat(), go["color"][1].GetFloat(), go["color"][2].GetFloat()));
		light->getParameters().set(Light::col, col);


	}
	//if this light has a shadowProjection, it means that this light projects shadows.
	if (go.HasMember("shadowProjection"))
	{
		const auto target = (go["shadowProjection"]["target"]).GetArray();
		glm::vec3 up(0, 1, 0);
		glm::vec3 targetVec(target[0].GetFloat(), target[1].GetFloat(), target[2].GetFloat());
		light->setProjection(targetVec, up);
		light->getShadowProjection()->setProjectionOrtho(5, 5, 0.1, 10);
	}
	light->setName("light");

	return light;
}

void tetraRender::SceneLoader::setResourceParam(Resource & resource, rapidjson::Value & resourceJSON)
{
	for (rapidjson::Value::MemberIterator i = resourceJSON.MemberBegin(); i != resourceJSON.MemberEnd(); i++)
	{
		std::string paramName = i->name.GetString();
		rapidjson::Value & parameter = i->value;
		//Now that we have a value we try to know what it is to be able to construct the corresponding parameter.
		if (parameter.IsArray())
		{
			//if the array is of size 3 and contains floats then we have a float vector.
			if (parameter.GetArray().Size() == 3 && parameter.GetArray()[0].IsNumber())
			{
				auto vecParam = parameter.GetArray();
				glm::vec3 vector(vecParam[0].GetFloat(), vecParam[1].GetFloat(), vecParam[2].GetFloat());
				resource.getParameters().set(paramName, vector);
			}
			else if (parameter.IsString())
			{
				resource.getParameters().set(paramName, parameter.GetString());
			}
		}
		else if (parameter.IsBool())
		{
			resource.getParameters().set(paramName, parameter.GetBool());
		}
		else if (parameter.IsNumber())
		{
			resource.getParameters().set(paramName, parameter.GetFloat());
		}


	}
}

Texture * SceneLoader::loadTexture(rapidjson::Value& texture)
{
	Texture *tex = (new Texture());
	if (texture.IsString())
	{
		tex->loadTexture(texture.GetString());
		tex->setName(texture.GetString());
	}
	else if(texture.IsObject())
	{
		setResourceParam(*tex, texture);
		if (texture.HasMember("type"))
		{
		
			if (texture["type"] == "CUBEMAP")
			{
				delete tex;
				CubeMap* map = new CubeMap();
				setResourceParam(*map, texture);
				std::string textureDir = texture["file"].GetString();
				textureDir += "/";
				std::vector<std::string> cubeSides = { textureDir + "right.jpg", textureDir + "left.jpg", textureDir + "top.jpg", textureDir + "bottom.jpg",  textureDir + "front.jpg",textureDir + "back.jpg" };
				map->loadTextures(cubeSides);
				map->getParameters().set("file", textureDir);
				tex = map;
			}
			else
			{
				std::string textureDir = texture["file"].GetString();
				tex->getParameters().set("file", textureDir);
				tex->loadTexture(textureDir);

			}
		}
		else
		{
			std::string name = texture["name"].GetString();
			tex->setName(name);
			std::string textureDir;
			if (!texture.HasMember("file"))
			{
				textureDir = name;
			}
			else
			{
				textureDir = texture["file"].GetString();
			}
		 
			tex->getParameters().set("file", textureDir);
			tex->loadTexture(textureDir);
		}
		std::string name = texture["name"].GetString();
		tex->setName(name);
	}
	
	
	return tex;
}


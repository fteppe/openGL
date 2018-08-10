#include "SceneSaver.h"
#include <fstream>


using namespace tetraRender;

SceneSaver::SceneSaver()
{
}


SceneSaver::~SceneSaver()
{
}



std::string tetraRender::SceneSaver::toJson(Scene & scene)
{
	
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);


	std::map<std::string, Material*> mats;
	std::map<std::string, Texture*> texs;
	

	for (auto mat : scene.getResources().getMaterials())
	{
		mats[mat.second->getName()] = mat.second.get();
	}

	writer.StartObject();
	writer.Key("gameObjects");
	writer.StartArray();
	for(auto go : scene.getGameObjects()->getChildren())
	addGameObjectToJSON(writer,go , mats);
	writer.EndArray();

	writer.Key("materials");
	writer.StartArray();
	for (auto mat : mats)
	{
		materialToJSON(writer, mat.second, texs);
	}
	writer.EndArray();

	writer.Key("shaders");
	writer.StartArray();
	for (auto mat : mats)
	{
		Shader* shader = mat.second->getShaderProgram().get();
		if (shader != nullptr)
		{
			shaderToJSON(writer, shader);
		}
	}
	writer.EndArray();

	writer.Key("textures");
	writer.StartArray();
	for (auto tex : texs)
	{
		textureToJSON(writer, tex.second);
	}
	writer.EndArray();
	writer.EndObject();
	return buffer.GetString();
}

void tetraRender::SceneSaver::saveToFile(Scene & scene, std::string filePath)
{
	std::string file = toJson(scene);
	std::ofstream out(filePath.c_str());
	out << file;
}

void tetraRender::SceneSaver::addGameObjectToJSON(rapidjson::Writer<rapidjson::StringBuffer>& writer, GameObject * gameObject,std::map<std::string, Material*>& mats )
{
	bool hadErrors = false;
	//Here are some reasons the fiel shouldn't be saved
	if (gameObject->getType() == GameObjectType::SOLID)
	{
		Solid* solid = static_cast<Solid*>(gameObject);
		auto mesh = solid->getMesh();
		if (mesh == nullptr)
		{
			hadErrors = true;
		}
		else
		{
			if (mesh->getFilePath().first == "hard")
			{
				hadErrors == true;
			}
		}

	}

	if (!hadErrors)
	{
		writer.StartObject();

		//depending on the game object there might be different things to add.
		switch (gameObject->getType())
		{
		case GameObjectType::SOLID:
		{

			Solid* solid = static_cast<Solid*>(gameObject);
			auto model = solid->getMesh()->getFilePath();
			writer.Key("model");
			writer.StartArray();

			writer.String(model.first.c_str());
			writer.String(model.second.c_str());
			writer.EndArray();
		}

		break;
		case GameObjectType::LIGHT:
			break;
		default:
			break;
		}
		auto mat = gameObject->getMaterial();
		if (mat != NULL && mat->getName().size()>0)
		{
			mats[mat->getName()] = mat.get();
			gameObject->getParameters().set(GameObject::material, std::string(mat->getName()));
		}
		auto renderTags = gameObject->getRenderTags();
		if (renderTags.size() > 0)
		{
			writer.Key("renderTags");
			writer.StartArray();
			for (RenderTag tag : renderTags)
			{
				std::string tagString = GameObject::getTagString(tag);
				if (tagString.size())
				{
					writer.String(tagString.c_str());
				}
			}
			writer.EndArray();

		}

		ParameterContainer& paramContainer = gameObject->getParameters();
		parameterToJSON(writer, paramContainer);

		writer.Key(GameObject::childrenField.c_str());
		writer.StartArray();
		for (auto child : gameObject->getChildren())
		{
			addGameObjectToJSON(writer, child, mats);
		}
		writer.EndArray();
		writer.EndObject();
	}

}

void tetraRender::SceneSaver::materialToJSON(Writer & writer, Material * mat, std::map<std::string, Texture*>& textures)
{
	if (mat->getShaderProgram() != nullptr)
	{
		writer.StartObject();

		mat->getParameters().set(Material::shaderField, std::string(mat->getShaderProgram()->getName()));
		parameterToJSON(writer, mat->getParameters());

		writer.Key("channels");
		writer.StartObject();
		for (auto pair : mat->getChannels())
		{
			std::shared_ptr<Texture> tex = pair.second;
			if (tex != nullptr)
			{
				textures[pair.second->getName()] = pair.second.get();
				writer.Key(pair.first.c_str());
				writer.String(pair.second->getName().c_str());
			}
		}
		writer.EndObject();
		writer.EndObject();
	}

}

void tetraRender::SceneSaver::textureToJSON(Writer & writer, Texture * tex)
{
	writer.StartObject();
	parameterToJSON(writer, tex->getParameters());
	writer.EndObject();
}

void tetraRender::SceneSaver::shaderToJSON(Writer & writer, Shader * shader)
{
	writer.StartObject();
	parameterToJSON(writer, shader->getParameters());
	writer.Key("vertex");
	writer.StartArray();
	for (auto shaderFile : shader->getShaderFiles())
	{
		if (shaderFile.second == GL_VERTEX_SHADER)
		{
			writer.String(shaderFile.first.c_str());
		}
	}
	writer.EndArray();
	writer.Key("fragment");
	writer.StartArray();
	for (auto shaderFile : shader->getShaderFiles())
	{
		if (shaderFile.second == GL_FRAGMENT_SHADER)
		{
			writer.String(shaderFile.first.c_str());
		}
	}
	writer.EndArray();
	writer.EndObject();
}

void tetraRender::SceneSaver::parameterToJSON(rapidjson::Writer<rapidjson::StringBuffer>& writer, ParameterContainer const & paramContainer)
{
	for (auto param : paramContainer.getParameters())
	{
		writer.Key(param.first.c_str());
		if (param.second == tetraRender::ParameterType::VEC3)
		{
			glm::vec3 paramVal = paramContainer.getVec3(param.first);
			writer.StartArray();
			writer.Double(paramVal.x);
			writer.Double(paramVal.y);
			writer.Double(paramVal.z);
			writer.EndArray();
		}
		else if (param.second == tetraRender::ParameterType::STRING)
		{
			writer.String(paramContainer.getString(param.first).c_str());
		}
		else if (param.second == tetraRender::ParameterType::FLOAT)
		{
			writer.Double(paramContainer.getFloat(param.first));
		}
		else if (param.second == tetraRender::ParameterType::BOOL)
		{
			writer.Bool(paramContainer.getBool(param.first));
		}
		else
		{
			writer.Null();
		}
	}
}

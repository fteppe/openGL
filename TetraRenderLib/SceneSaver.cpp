#include "SceneSaver.h"


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

	addGameObjectToJSON(writer, scene.getGameObjects());

	return buffer.GetString();
}

void tetraRender::SceneSaver::addGameObjectToJSON(rapidjson::Writer<rapidjson::StringBuffer>& writer, GameObject * gameObject)
{
	writer.StartObject();
	writer.Key("type");
	//depending on the game object there might be different things to add.
	switch (gameObject->getType())
	{
	case GameObjectType::SOLID:
		writer.String("solid");
		break;
	case GameObjectType::LIGHT:
		writer.String("light");
		break;
	default:
		writer.Null();
		break;
	}


	ParameterContainer& paramContainer = gameObject->getParameters();
	parameterToJSON(writer, paramContainer);

	writer.Key(GameObject::childrenField.c_str());
	writer.StartArray();
	for (auto child : gameObject->getChildren())
	{
		addGameObjectToJSON(writer, child);
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

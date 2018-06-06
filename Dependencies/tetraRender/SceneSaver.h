#pragma once
#include <string>
#include "Scene.h"

typedef rapidjson::Writer<rapidjson::StringBuffer> Writer;
namespace tetraRender
{
	class SceneSaver
	{
	public:
		SceneSaver();
		~SceneSaver();
		std::string toJson(Scene& scene);
		void saveToFile(Scene& scene, std::string filePath);
		void addGameObjectToJSON(Writer& writer, GameObject * gameObject, std::map<std::string, Material*>& mats);
		void materialToJSON(Writer& writer, Material* mat, std::map<std::string, Texture*>& textures);
		void textureToJSON(Writer& writer, Texture* tex);
		void shaderToJSON(Writer& writer, Shader* shader);
		void parameterToJSON(Writer& writer, ParameterContainer const & params);
	};
}



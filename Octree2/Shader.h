#pragma once
#include "Texture.h"
class Scene;
class Solid;
#include <string>
#include <vector>
#include <map>
#include <glew\glew.h>
#include <glm/matrix.hpp>
const std::string shaderDir = "shaders/";

class Shader
{
public:
	Shader();
	Shader(std::string vertex, std::string fragment);
	//This constructor allows several files to be linked in a single program.
	Shader(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders);
	//Shader(const Shader& shader);
	~Shader();
	unsigned int getProgram() const;
	virtual void setProgramInformation(Scene const& scene,Solid const& object);
	void sendTexChannels(std::map<std::string, Texture*> textures);
protected:
	void compileShader(GLuint shader, std::string shaderPath);
	void linkProgram();
	unsigned int program;



};


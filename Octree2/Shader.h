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
	void setDiffuse(Texture& tex);
	//We can set any texture channel for this program.
	void setChannel(Texture& tex, std::string channelName);
	virtual void setProgramInformation(Scene const& scene,Solid const& object);
	void sendTexChannels(std::map<std::string,std::shared_ptr<Texture>> textures);
protected:
	//send all the texture channels to the program.
	void sendTexChannels();
	void compileShader(GLuint shader, std::string shaderPath);
	void linkProgram();
	unsigned int program;
	//a map of the different texture channels, the key is the name of the channel.
	std::map<std::string, Texture> texChannels;


};


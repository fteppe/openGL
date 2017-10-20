#pragma once
#include "Texture.h"
class Scene;
class Solid;
#include <string>
#include <vector>
#include <map>
#include <glew\glew.h>
#include <glm/matrix.hpp>
class Shader
{
public:
	Shader();
	Shader(std::string vertex, std::string fragment);
	//Shader(const Shader& shader);
	~Shader();
	unsigned int getProgram() const;
	void setDiffuse(Texture tex);
	virtual void setProgramInformation(Scene const& scene,Solid const& object);
protected:
	//send all the texture channels to the program.
	void sendTexChannels();
	void compileShader(GLuint shader, std::string shaderPath);

	unsigned int program;
	//a map of the different texture channels, the key is the name of the channel.
	std::map<std::string, Texture> texChannels;


};


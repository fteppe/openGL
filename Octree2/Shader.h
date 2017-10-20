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
	unsigned int getProgram();
	//void setVertex(std::vector<std::vector<GLfloat>> vertices,std::vector<int> index, std::vector<int> nbData);
	void setDiffuse(Texture tex);
	void setProgramInformation(Scene const& scene,Solid const& object);
	//void draw();

	//When the same shader is applied to different objects, it is necessary to create new VBAOs for it to work. In the future it is possible that a shader doesn't have the VBAos but the object does?
	//it would make more sense.
	void applyToNewObject();
protected:
	//send all the texture channels to the program.
	void sendTexChannels();
	void compileShader(GLuint shader, std::string shaderPath);

	unsigned int program;
	//a map of the different texture channels, the key is the name of the channel.
	std::map<std::string, Texture> texChannels;


};


#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glew/glew.h>
#include <glm\gtc\type_ptr.hpp>


Shader::Shader()
{
}

Shader::Shader(std::string vertex, std::string fragment)
{

	vertex = "shaders/" + vertex;
	fragment = "shaders/" + fragment;
	std::string lightCalc = "shaders/lightCalc.frag";

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint lumDiffuseCalc = glCreateShader(GL_FRAGMENT_SHADER);
	//compiling both shaders
	compileShader(vertexId, vertex);
	compileShader(fragmentId, fragment);
	compileShader(lumDiffuseCalc, lightCalc);

	program = glCreateProgram();
	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);
	glAttachShader(program, lumDiffuseCalc);
	//once they are linked to a program they are deleted
	linkProgram();
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	glDeleteShader(lumDiffuseCalc);
	
}




Shader::~Shader()
{
	glDeleteProgram(program);
}

unsigned int Shader::getProgram() const
{
	return program;
}


void Shader::setDiffuse(Texture& tex)
{
	//if there is no diffuse texture, we extend the size of the array
	texChannels["diffuse"] = tex;
}

void Shader::setChannel(Texture & tex, std::string channelName)
{
	texChannels[channelName] = tex;
}

void Shader::setProgramInformation(Scene const& scene, Solid const& object)
{
	glUseProgram(program);
	sendTexChannels();
	Camera cam = scene.getCam();
	Light light = scene.getLight();
	//We get the light data;
	std::vector<float> lightData(light.getDataArray());
	//we get the camera space and calulculate the projection that will be done to all the vertices
	glm::mat4 cameraSpace = cam.getProjection();
	glm::mat4 objectSpace = object.getObjectSpace();
	glm::mat4 worldSpace = cameraSpace * objectSpace;
	//the projection matrix sent to the shader
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, glm::value_ptr(worldSpace));
	//the objectspace that can be used to calculate lights or the posiiton of a vertex to a point. We send it to the shader.
	glUniformMatrix4fv(glGetUniformLocation(program, "objectSpace"), 1, false, glm::value_ptr(objectSpace));
	//we send the light data to the shader, for now we can handle only one light
	glUniform1fv(glGetUniformLocation(program, "light"), lightData.size(), &lightData[0]);

}

void Shader::sendTexChannels()
{
	glUseProgram(program);
	//iterating through the map of channels.
	int i = 0;
	for (std::map<std::string, Texture>::iterator it = texChannels.begin(); it != texChannels.end(); it++)
	{
		//we send to the program the channel, with it's name and the texture unit.
		it->second.applyTexture(program, it->first, i);
		i++;
	}
}




void Shader::compileShader(GLuint shader, std::string shaderPath)
{
	std::ifstream shaderSource(shaderPath);
	std::string source = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());

	const char* shaderChar = source.c_str();
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<shaderPath<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << source;
	}
}

void Shader::linkProgram()
{
	glLinkProgram(program);

	int  success = 0;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK FAILED\n" << infoLog << std::endl;
	}
}

#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glew/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Solid.h"
#include "Light.h"
#include "PreprocessorShader.h"
using namespace tetraRender;

Shader::Shader()
{
	highestTextureUnitUsed = 0;
	setName("shader");
	parametersContainer.set("type", std::string("Shader"));
}


Shader::Shader(std::string vertex, std::string fragment) : Shader()
{
	std::vector<std::string> vertexShaders;
	vertexShaders.push_back(vertex);
	std::vector<std::string> fragmentShaders;
	fragmentShaders.push_back(fragment);
		std::vector<GLuint> vertexs;
	std::vector<GLuint> fragments;
	//We create the shaders
	for (int i = 0; i < vertexShaders.size(); i++)
	{
		shaderFiles.push_back(std::pair<std::string, GLenum>(vertexShaders[i], GL_VERTEX_SHADER));
	}
	for (int i = 0; i < fragmentShaders.size(); i++)
	{
		shaderFiles.push_back(std::pair<std::string, GLenum>(fragmentShaders[i], GL_FRAGMENT_SHADER));
	}
	compileAll();
	getUniformLocations();

}

Shader::Shader(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders) : Shader() 
{
	std::vector<GLuint> vertexs;
	std::vector<GLuint> fragments;
	//We create the shaders
	for (int i = 0; i < vertexShaders.size(); i++)
	{
		shaderFiles.push_back(std::pair<std::string, GLenum>(vertexShaders[i], GL_VERTEX_SHADER));
	}
	for (int i = 0; i < fragmentShaders.size(); i++)
	{
		shaderFiles.push_back(std::pair<std::string, GLenum>(fragmentShaders[i], GL_FRAGMENT_SHADER));
	}
	compileAll();
	getUniformLocations();
}





Shader::~Shader()
{
	glDeleteProgram(program);
}

void tetraRender::Shader::compileAll()
{
	std::vector<GLuint> shaders;
	for (auto shaderFile : shaderFiles)
	{
		shaders.push_back(glCreateShader(shaderFile.second));

	}
	int i = 0;
	for (auto shaderFile : shaderFiles)
	{
		compileShader(shaders[i], shaderDir + shaderFile.first);
		i++;
	}
	if (program != 0)
	{
		glDeleteProgram(program);
	}
	program = glCreateProgram();
	use();
	//we attach each shader to the program
	i = 0;
	for (auto shaderFile : shaderFiles)
	{
		glAttachShader(program, shaders[i]);
		i++;
	}
	i = 0;
		//linking
	std::cout << "Linking" << std::endl;
	linkProgram();
	for (auto shaderFile : shaderFiles)
	{
		glDeleteShader(shaders[i]);
		i++;
	}
	this->uniforms.clear();
}

unsigned int Shader::getProgram() const
{
	return program;
}





void Shader::setProgramInformation(tetraRender::Scene& scene, Solid const& object)
{
	glUseProgram(program);
	Camera cam = scene.getCam();
	//Light light = scene.getLight();
	//We get the light data;
	//std::vector<float> lightData(light.getDataArray());
	//we get the camera space and calulculate the projection that will be done to all the vertices
	glm::mat4 cameraSpace = cam.getProjection();
	glm::mat4 objectSpace = object.getmodelMatrix();
	glm::mat4 worldSpace = cameraSpace * objectSpace;
	//the projection matrices sent to the shader
	sendMatrix4("objectSpace", objectSpace);
	sendMatrix4("mvp", worldSpace);
	sendFloat("near", cam.getNearFarPlanes().x);
	sendFloat("far", cam.getNearFarPlanes().y);

	glm::vec3 camPos = cam.getPos();
	glm::mat4 cameraSpace = cam.getProjection();
	glm::mat4 world2Obj = glm::inverse(object.getmodelMatrix());
	sendMatrix4("world2obj", world2Obj);
	//sending the camera position
	if (uniforms.find("camPos") == uniforms.end())
	{
		uniforms["camPos"] = glGetUniformLocation(program, "camPos");
	}
	//std::cout << camPos.x<<" "<< camPos.y<<" "<< camPos.z << std::endl;
	glUniform3f(uniforms["camPos"], camPos.x, camPos.y, camPos.z);
	float time = scene.getElapsedTime();
	sendFloat("time", time);
}

void Shader::sendTexChannels(std::map<std::string, std::shared_ptr<Texture>> textures)
{
	glUseProgram(program);
	//iterating through the map of channels.
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		//we send to the program the channel, with it's name and the texture unit.
		if (it->second != nullptr)
		{
			sendTexture(it->first, it->second);
		}
	}

}

void tetraRender::Shader::resetTextureUnitCount()
{
	for (int i = 0; i < highestTextureUnitUsed; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	this->highestTextureUnitUsed = 0;
}

void tetraRender::Shader::use()
{
	glUseProgram(program);
}

void tetraRender::Shader::setShaderFiles(std::vector<std::pair<std::string, GLenum>> shaderFiles)
{
	this->shaderFiles = shaderFiles;
}

std::vector<std::pair<std::string, GLenum>> tetraRender::Shader::getShaderFiles()
{
	return shaderFiles;
}

void Shader::compileShader(GLuint shader, std::string shaderPath)
{
	std::ifstream shaderSource(shaderPath);
	std::vector<std::string> includes;
	std::string source = PreprocessorShader::processFile(shaderPath, includes);
	if (source.size() == 0)
	{
		std::cout << __FILE__ << " " << __LINE__ << "empty shader File" <<shaderPath<< std::endl;
	}

	const char* shaderChar = source.c_str();
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<shaderPath<<"::COMPILATION_FAILED\n" << infoLog;
		std::cout << "inclusion order : \n";
		for (unsigned i = 0; i < includes.size(); i++)
		{
			std::cout << i<<" :: "<<includes[i] << std::endl;
		}
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

void Shader::getUniformLocations()
{
	glUseProgram(program);
	uniforms["mvp"] = glGetUniformLocation(program, "mvp");
	uniforms["objectSpace"] = glGetUniformLocation(program, "objectSpace");
	//uniforms["light"] = glGetUniformLocation(program, "light");
}

void Shader::sendMatrix4(std::string name, glm::mat4 matrix)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(matrix));
}

void Shader::sendFloat(std::string name, float floatIn)
{
	//floatIn = 0.5;

	GLint loc = getUniformLocation(name);
	glUniform1f(loc, floatIn);

}

void tetraRender::Shader::sendInt(std::string name, int intIn)
{
	GLint pos = getUniformLocation(name);
	glUniform1i(pos, intIn);
}

void tetraRender::Shader::sendVec3(std::string name, glm::vec3 vec)
{
	GLint pos = getUniformLocation(name);
	glUniform3f(pos, vec.x, vec.y, vec.z);
}

void tetraRender::Shader::sendLight(std::string name, tetraRender::Light light)
{
	//Since the light is a struct we need to send each component.
	sendVec3(name + ".pos",light.getmodelMatrix() * glm::vec4(light.getPos(),1));
	//sendVec3(name + ".pos", glm::vec3(0,0,1));
	sendVec3(name + ".color", light.getParameters().getVec3(Light::col));
	sendFloat(name + ".intensity", light.getParameters().getFloat(Light::intensity));
	//By default we consider that no light has a shadow, and we change that when we send the shadows datas.
	sendInt(name + ".shadowIndex", -1);
	//GLint posIntensity = getUniformLocation(name + ".intensity");
}

void tetraRender::Shader::sendTexture(std::string channelName, std::shared_ptr<Texture> tex)
{
	
	if (uniforms.find(channelName) == uniforms.end())
	{
		uniforms[channelName] = glGetUniformLocation(program, channelName.c_str());
	}
	
	tex->applyTexture(this->program, uniforms[channelName], highestTextureUnitUsed);
	highestTextureUnitUsed++;
}

GLint tetraRender::Shader::getUniformLocation(std::string uniform)
{
	GLint loc;
	if (uniforms.find(uniform) == uniforms.end())
	{
		loc = glGetUniformLocation(program, uniform.c_str());
		uniforms[uniform] = loc;
	}
	else
	{
		loc = uniforms[uniform];
	}
	return loc;
}

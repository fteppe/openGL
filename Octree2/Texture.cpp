#include "stdafx.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glew/glew.h>
#include <iostream>

Texture::Texture()
{
	glGenTextures(1, &textureID);
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::loadTexture(std::string textureName)
{
	texturePath = textureName;
	data =  stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
	textureData = std::vector<unsigned char>(data, data+width* height* nrChannels);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Depending on the number of channels the texture is loaded differently.
	if (nrChannels == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	//once the texture has been loaded we free it from the ram where it is no longer used.
	stbi_image_free(data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::applyTexture(GLuint program, std::string varName, int textureUnit)
{
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glUseProgram(program);
	//data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (1)
	{		
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureID);
		//We will need to change that to accomodate for several channels.
		glUniform1i(glGetUniformLocation(program, varName.c_str()), textureUnit); // set it manually
	}
	else
	{
		std::cout << "no texture to load" << std::endl;
	}

}


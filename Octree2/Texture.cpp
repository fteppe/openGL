#include "stdafx.h"
#include "Texture.h"

#include "stb_image.h"
#include <glew/glew.h>
#include <iostream>

Texture::Texture()
{
	glGenTextures(1, &textureID);
	textureType = GL_TEXTURE_2D;
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind()
{
	glBindTexture(textureType, this->textureID);
}

void Texture::loadTexture(std::string textureName)
{
	unsigned char* data;

	texturePath = textureName;
	std::cout << "loading " << textureName << std::endl;
	data =  stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
	//textureData = std::vector<unsigned char>(data, data+width* height* nrChannels);
	glBindTexture(textureType, textureID);
	setTextureParameters();
	//Depending on the number of channels the texture is loaded differently.
	if (data)
	{
		loadImage(textureType, width, height, nrChannels, data);
	}
	else
	{
		std::cout << "no texture" << std::endl;
	}
	
	//once the texture has been loaded we free it from the ram where it is no longer used.
	stbi_image_free(data);
	glGenerateMipmap(textureType);
	
	
	std::cout << "done loading " << textureName << std::endl;
}

void Texture::loadTextures(std::vector<std::string> textureName)
{
}

void Texture::applyTexture(GLuint program, GLuint texturePos, int textureUnit)
{
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glUseProgram(program);
	//data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (1)
	{		
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(textureType, textureID);

		glUniform1i(texturePos, textureUnit); // set it manually
	}
	else
	{
		std::cout << "no texture to load" << std::endl;
	}

}

void Texture::readData()
{
	glBindTexture(textureType, textureID);
	unsigned char* data = new unsigned char[width * height * nrChannels];
	glGetTexImage(textureType, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete data;
}

GLuint Texture::getId()
{
	return textureID;
}

void Texture::loadImage(GLuint texType, int width, int height, int nrChannels, unsigned char * data)
{
	this->width = width;
	this->height = height;
	this->nrChannels = nrChannels;
	if (nrChannels == 1)
	{
		glTexImage2D(texType, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 2)
	{
		glTexImage2D(texType, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 3)
	{
		glTexImage2D(texType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(texType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}

void Texture::setTextureParameters()
{
	
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}


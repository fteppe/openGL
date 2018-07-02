#include "stdafx.h"
#include "Texture.h"

#include "stb_image.h"
#include <glew/glew.h>
#include <iostream>

using namespace tetraRender;

const std::string Texture::file = "file";
const std::string Texture::gammaCorrected = "gammaCorrected";
const std::string Texture::HDRvalue = "HDRValues";
const std::string Texture::repeat = "repeat";

Texture::Texture()
{
	glGenTextures(1, &textureID);
	textureType = GL_TEXTURE_2D;
	width = 0;
	height = 0;
	nrChannels = 0;
	parametersContainer.set(gammaCorrected, false);
	parametersContainer.set(HDRvalue, false);
	parametersContainer.set(repeat, true);
	parametersContainer.set(file, std::string(""));
	parametersContainer.set("type", std::string("TEXTURE2D"));
	setName("texture");
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind()
{
	glBindTexture(textureType, this->textureID);
}

void Texture::loadTexture(std::string textureName, GLenum textureTypeIn)
{
	void* data = readFile(textureName);
	if (data)
	{
		if (parametersContainer.getBool(Texture::HDRvalue))
		{
			data = stbi_loadf(textureName.c_str(), &width, &height, &nrChannels, 0);
			loadHDR(textureTypeIn, width, height, nrChannels, (float*)data);
		}
		else
		{
			data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
			loadImage(textureTypeIn, width, height, nrChannels, (unsigned char*)data);
		}
		glGenerateMipmap(textureTypeIn);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "no data in " + textureName << std::endl;
	}
}

void * tetraRender::Texture::readFile(std::string textureName)
{
	//parametersContainer.set(file, textureName);
	glBindTexture(textureType, textureID);
	void* data = nullptr;
	//If we have no name for our texture we create an empty one
	if (textureName.size() == 0)
	{
		glBindTexture(textureType, textureID);
		glTexImage2D(textureType, 0, GL_RED, 0, 0, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		//unsigned char* data = nullptr;

		texturePath = textureName;
		std::cout << "loading " << textureName << std::endl;
		//if we are loading a HDR texture then we need to load it differently
		if (parametersContainer.getBool(Texture::HDRvalue))
		{
			data = stbi_loadf(textureName.c_str(), &width, &height, &nrChannels, 0);
			//loadHDR(textureType, width, height, nrChannels, dataFloat);
			//stbi_image_free(dataFloat);
		}
		else
		{
			data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);

		}
		if (data)
		{
			//loadImage(textureType, width, height, nrChannels, data);
			//stbi_image_free(data);

		}
		else
		{
			std::cout << "no texture" << std::endl;
		}
		//textureData = std::vector<unsigned char>(data, data+width* height* nrChannels);
		std::cout << "done loading " << textureName << std::endl;

		glBindTexture(textureType, textureID);

		//Depending on the number of channels the texture is loaded differently.

		setTextureParameters();
		//once the texture has been loaded we free it from the ram where it is no longer used.
		//glGenerateMipmap(textureType);
	}


	return data;
}


void Texture::applyTexture(GLuint program, GLuint texturePos, int textureUnit)
{
	
	glUseProgram(program);
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

void Texture::setDataType(GLenum dataType)
{
	this->dataType = dataType;
}

void Texture::setFormat(GLenum format)
{
	internalFormat = format;
	if (format == GL_DEPTH_STENCIL)
	{
		nrChannels = 4;
	}
}

void Texture::setDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Texture::readData()
{
	/*glBindTexture(textureType, textureID);
	unsigned char* data = new unsigned char[width * height * nrChannels];
	GLenum err = glGetError();
	glGetTexImage(textureType, 0, this->internalFormat, this->dataType, data);
	err = glGetError();
	delete data;
	*/
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
		GLenum internalFormat = GL_RGB;
		if (parametersContainer.getBool(gammaCorrected))
		{
			internalFormat = GL_SRGB;
		}
		glTexImage2D(texType, 0, internalFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(texType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}

void tetraRender::Texture::loadHDR(GLuint textureType, int width, int height, int channels, float * data)
{
	this->width = width;
	this->height = height;
	this->nrChannels = nrChannels;
	if (nrChannels == 1)
	{
		glTexImage2D(textureType, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, data);
	}
	else if (nrChannels == 2)
	{
		glTexImage2D(textureType, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, data);
	}
	else if (nrChannels == 3)
	{
		glTexImage2D(textureType, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
	}
	else
	{
		glTexImage2D(textureType, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, data);
	}
}

void tetraRender::Texture::setGamma(bool needsGammaCorrection)
{
	parametersContainer.set(gammaCorrected, needsGammaCorrection);

}

void tetraRender::Texture::update()
{
	loadTexture(parametersContainer.getString(file));
}

void Texture::setTextureParameters()
{
	if (parametersContainer.getBool(repeat))
	{
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}


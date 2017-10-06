#include "stdafx.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glew/glew.h>
#include <iostream>

Texture::Texture() : isLoaded(0)
{
	
	glGenTextures(1, &textureID);
}


Texture::~Texture()
{
}

void Texture::loadTexture(std::string textureName)
{
	texturePath = textureName;
	unsigned char * data =  stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
	textureData = std::vector<unsigned char>(data, data+width* height* nrChannels);

	if (textureData.size())
	{
		isLoaded = 1;

		glBindTexture(GL_TEXTURE_2D, textureID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//this is in case the number of pixel is odd.
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		if (textureData.size())
		{

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &textureData[0]);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "no texture to load" << std::endl;
		}
	}
	else
	{
		isLoaded = 0;
		std::cout << "loading error, Texture.cpp" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::applyTexture(GLuint program, std::string varName)
{
	//wrong for now it means we can have onnly one channel
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (textureData.size())
	{
		
		
		glUniform1i(glGetUniformLocation(program, varName.c_str()), 0); // set it manually
	}
	else
	{
		std::cout << "no texture to load" << std::endl;
	}

}

unsigned int Texture::textureLoaded() const
{
	return isLoaded;
}


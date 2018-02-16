#include "stdafx.h"
#include "CubeMap.h"
#include <iostream>

CubeMap::CubeMap() : Texture()
{
	textureType = GL_TEXTURE_CUBE_MAP;
}

void CubeMap::loadTextures(std::vector<std::string > cubeMapElements)
{

	int width, height, nrChannels;
	unsigned char* data;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	setTextureParameters();
	//the regular order is : right, left, top, bottom, back, front
	for (int i = 0 ; i < cubeMapElements.size() ; i++)
	{
		std::string textureName = cubeMapElements[i];
		std::cout << "loading " << textureName << std::endl;
		data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
		GLint texType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		loadImage(texType, width, height, nrChannels, data);
		stbi_image_free(data);
		std::cout << "done loading " << textureName << std::endl;
	}
	
	//texturePath = textureName;
	
	//textureData = std::vector<unsigned char>(data, data+width* height* nrChannels);

	
	//Depending on the number of channels the texture is loaded differently.
	
	//once the texture has been loaded we free it from the ram where it is no longer used.
	
	glGenerateMipmap(textureType);


	
}


CubeMap::~CubeMap()
{
}

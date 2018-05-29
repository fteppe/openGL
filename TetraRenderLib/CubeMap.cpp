#include "stdafx.h"
#include "CubeMap.h"
#include <iostream>

using namespace tetraRender;

CubeMap::CubeMap() : Texture()
{
	textureType = GL_TEXTURE_CUBE_MAP;
}

void CubeMap::loadTextures(std::vector<std::string > cubeMapElements)
{

	int width, height, nrChannels;
	unsigned char* data;
	glBindTexture(textureType, textureID);
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
	
	glGenerateMipmap(textureType);
}


CubeMap::~CubeMap()
{
}

#include "stdafx.h"
#include "CubeMap.h"
#include <iostream>

using namespace tetraRender;

CubeMap::CubeMap() : Texture()
{
	textureType = GL_TEXTURE_CUBE_MAP;
	parametersContainer.set("type", std::string("CUBEMAP"));
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
		GLint texType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		loadTexture(textureName, texType);
	}
	
	glGenerateMipmap(textureType);
}

void tetraRender::CubeMap::update()
{
	std::string textureDir = getParameters().getString(file);
	std::vector<std::string> cubeSides = { textureDir + "right.jpg", textureDir + "left.jpg", textureDir + "top.jpg", textureDir + "bottom.jpg",  textureDir + "front.jpg",textureDir + "back.jpg" };
	loadTextures(cubeSides);
}


CubeMap::~CubeMap()
{
}

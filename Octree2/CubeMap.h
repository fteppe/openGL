#pragma once
#include "Texture.h"
class CubeMap :
	public Texture
{
public:
	CubeMap();
	void loadTextures(std::vector<std::string > cubeMapElements);
	~CubeMap();
};


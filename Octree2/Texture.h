#pragma once
#include <vector>
#include <string>
class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture();

private:
	int textureID;
	std::string texturePath;
	std::vector<std::vector<std::vector<char>>> textureData;
};


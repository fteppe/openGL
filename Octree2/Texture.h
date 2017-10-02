#pragma once
#include <vector>
#include <string>
#include <glew\glew.h>


class Texture
{
public:
	Texture();
	~Texture();
	unsigned int textureLoaded() const;
	void loadTexture(std::string textureName);
	void applyTexture(GLuint program, std::string varName);

private:
	GLuint textureID;
	int isLoaded;
	std::string texturePath;
	std::vector<unsigned char> textureData;
	int width, height, nrChannels;
};


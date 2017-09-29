#pragma once
#include <vector>
#include <string>
#include <glew\glew.h>


class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture(std::string textureName);
	void applyTexture(GLuint program, std::string varName);

private:
	GLuint textureID;
	std::string texturePath;
	std::vector<unsigned char> textureData;
	unsigned char* data;
	int width, height, nrChannels;
};


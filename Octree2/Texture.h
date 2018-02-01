#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glew\glew.h>


class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture(std::string textureName);
	void applyTexture(GLuint program, std::string varName, int textureUnit);

private:
	GLuint textureID;
	std::string texturePath;
	std::vector<unsigned char> textureData;
	unsigned char* data;
	int width, height, nrChannels;
};


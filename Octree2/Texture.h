#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glew\glew.h>
#include "stb_image.h"

#ifndef  STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !  


class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture(std::string textureName);
	virtual void loadTextures(std::vector<std::string> textureName);
	void applyTexture(GLuint program, GLuint texturePos, int textureUnit);

protected:
	void loadImage(GLuint textureType, int width,int height,int channels, unsigned char * data );
	void setTextureParameters();
	GLuint textureID;
	GLint textureType;
	std::string texturePath;
	//std::vector<unsigned char> textureData;


};


#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glew/glew.h>
#include "stb_image.h"
#include "Resource.h"

#ifndef  STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !  


namespace tetraRender
{
	class Texture : public tetraRender::Resource
	{
	public:
		Texture();
		~Texture();
		void bind();
		void loadTexture(std::string textureName);
		void applyTexture(GLuint program, GLuint texturePos, int textureUnit);
		void setDataType(GLenum dataType);
		void setFormat(GLenum format);
		void setDimensions(int width, int height);
		void loadImage(GLuint textureType, int width, int height, int channels, unsigned char * data);
		void setGamma(bool needsGammaCorrection);
		/*
		*Gets all texture data back from the GPU.
		*/
		void readData();
		GLuint getId();

	protected:

		void setTextureParameters();
		GLuint textureID;
		GLint textureType;
		GLenum dataType;
		GLenum internalFormat;
		int width, height, nrChannels;
		std::string texturePath;
		static const std::string file;
		static const std::string gammaCorrected;
		static const std::string HDRvalue;
		//std::vector<unsigned char> textureData;
	};
}


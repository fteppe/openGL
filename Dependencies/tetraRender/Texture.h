#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glew/glew.h>
#include "stb_image.h"
#include "Resource.h"
#include <atomic>
#include <future>

#ifndef  STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !  


namespace tetraRender
{
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture();
		void bind();
		void loadTexture(std::string textureName, GLenum textureType = GL_TEXTURE_2D);
		void applyTexture(GLuint program, GLuint texturePos, int textureUnit);
		void setDataType(GLenum dataType);
		void setFormat(GLenum format);
		void setDimensions(int width, int height);
		void loadImage(GLuint textureType, int width, int height, int channels, unsigned char * data);
		void loadHDR(GLuint textureType, int width, int height, int channels, float * data);
		void setGamma(bool needsGammaCorrection);
		void update();
		/*
		*Gets all texture data back from the GPU.
		*/
		void readData();
		GLuint getId();
		static const std::string file;
		static const std::string gammaCorrected;
		static const std::string HDRvalue;
		static const std::string repeat;


	protected:
		void* readFile(std::string textureName);

		void asyncLoadTexture(std::string textureName, GLenum textureType = GL_TEXTURE_2D);
		void asyncLoadCheck();

		bool isLoading;
		std::future<void*> data;

		void setTextureParameters();
		GLuint textureID;
		GLint textureType;
		GLenum dataType;
		GLenum internalFormat;
		std::atomic<int> width, height, nrChannels;
		std::string texturePath;

		//std::vector<unsigned char> textureData;
	};
}


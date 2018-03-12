#pragma once
#include <memory>
#include "Texture.h"
#include "Common.h"
namespace tetraRender
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(EngineEnum frameType);
		~FrameBuffer();
		/*
		*This function attach a texture to this frame buffer so colors are rendered here.
		*The texture can then be used later in the program as a regular texture.
		*/
		void attachOutputTexture(std::shared_ptr<Texture> texture);
		/*
		*sets the render target to this framebuffer
		*/
		void renderToThis();
		/*
		*set the rendering target to the screen.
		*/
		void renderToScreen();
		//We add another output color.
		void addColorOutputTexture(std::shared_ptr<Texture> texture);
		void setDepthTexture(std::shared_ptr<Texture> texture);

		void bind();
		void unbind();
	private:
		unsigned int height, width;
		GLuint frameBufferId;
		GLuint renderBufferId;
		//the color textures of this frameBuffer
		std::vector< std::shared_ptr<Texture>> colorTextures;
		//when we add a texture for a color output, we need to say what kind it is.
		//for no it will be GL_COLOR_ATTACHMENTn
		std::vector< GLenum> colorLayout;
		//if there is a texture attached to it, this is where the frame buffer will be.
		std::shared_ptr<Texture> depthBufferTexture;

		//To be deprecated.
		std::shared_ptr<Texture> outputTexture;
	};
}


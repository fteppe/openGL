#pragma once
#include <memory>
#include "Texture.h"
#include "Common.h"
#include <glm/vec2.hpp>
namespace tetraRender
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(EngineEnum frameType);
		~FrameBuffer();

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

		//If we want our framebuffer to have HDR values we set it to true, by doing so all color output textures must be HDR as well.
		void setHDR(bool hdrVal);

		void bind();
		void bind(GLenum target);
		void unbind();
		void clear(tetraBitField mask);
		glm::vec2 getSize();
	private:
		unsigned int height, width;
		bool isHDR;
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


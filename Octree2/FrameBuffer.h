#pragma once
#include <memory>
#include "Texture.h"
class FrameBuffer
{
public:
	FrameBuffer();
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
private:
	unsigned int height, width;
	GLuint frameBufferId;
	GLuint renderBufferId;
	std::shared_ptr<Texture> outputTexture;
};


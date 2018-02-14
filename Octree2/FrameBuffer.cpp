#include "stdafx.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &frameBufferId);
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, height, width);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &frameBufferId);
	glDeleteRenderbuffers(1, &renderBufferId);
}

void FrameBuffer::attachOutputTexture(std::shared_ptr<Texture> texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	outputTexture = texture;
	outputTexture->bind();
	//We don't send any data to this texture sinec the data will come from the rendering.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferId, 0);

	//we set the frame buffer back to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::renderToThis()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
}

void FrameBuffer::renderToScreen()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

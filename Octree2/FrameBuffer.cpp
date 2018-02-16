#include "stdafx.h"
#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer()
{
	width = 1024;
	height = 720;
	glGenFramebuffers(1, &frameBufferId);

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	renderToScreen();
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	outputTexture->loadImage(GL_TEXTURE_2D, width, height, 3, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture->getId(), 0);


	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);


	GLenum error  = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//we set the frame buffer back to the screen
	renderToScreen();
}

void FrameBuffer::renderToThis()
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	glClearColor(0.5, 1, 1,1);
}

void FrameBuffer::renderToScreen()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 0);
}

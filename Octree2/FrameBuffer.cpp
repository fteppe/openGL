#include "stdafx.h"
#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer()
{
	//TODO: not have these has hard coded values.
	width = WIDTH;
	height = HEIGHT;
	glGenFramebuffers(1, &frameBufferId);

	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

	renderToScreen();
}

FrameBuffer::FrameBuffer(EngineEnum frameType)
{
	width = WIDTH;
	height = HEIGHT;
	frameBufferId = 0;
	renderBufferId = 0;
}


FrameBuffer::~FrameBuffer()
{
	//In case we have a buffer that writes to the screen we don't want to delete it, I'm not sure of the behavior deleting the screen frame buffer accross plateforms.
	if (frameBufferId != 0)
	{
		glDeleteFramebuffers(1, &frameBufferId);
		glDeleteRenderbuffers(1, &renderBufferId);
	}
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

	//We tell here which color buffers we want to render. if we have no texture we always render one buffer.
	unsigned colorBuffersNumber = colorLayout.size();
	std::vector<GLenum> buffers = colorLayout;
	if (colorBuffersNumber == 0)
	{
		buffers = { GL_COLOR_ATTACHMENT0 };
	}
	glDrawBuffers(buffers.size(), &buffers[0]);
}

void FrameBuffer::renderToScreen()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 0);
}

void FrameBuffer::addColorOutputTexture(std::shared_ptr<Texture> texture)
{
	//we bind the frame buffer.
	renderToThis();
	//outputTexture = texture;
	texture->bind();
	//We associate a 2D texture to the one we bound.
	//We don't send any data to this texture sinec the data will come from the rendering.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//we send the image to the graphic card
	texture->loadImage(GL_TEXTURE_2D, width, height, 3, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//the color attachment determines the layout in the fragment shader of the outputs.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->colorTextures.size(), GL_TEXTURE_2D, texture->getId(), 0);
	//We add this type of texture to the layout.
	this->colorLayout.push_back(GL_COLOR_ATTACHMENT0 + this->colorTextures.size());
	colorTextures.push_back(texture);

	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//we set the frame buffer back to the screen
	renderToScreen();
}

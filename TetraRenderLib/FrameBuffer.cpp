#include "stdafx.h"
#include "FrameBuffer.h"
#include <iostream>

using namespace tetraRender;

FrameBuffer::FrameBuffer()
{
	//TODO: not have these has hard coded values.
	width = WIDTH;
	height = HEIGHT;
	isHDR = false;
	glGenFramebuffers(1, &frameBufferId);

	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);
	glEnable(GL_DEPTH_TEST);
	unbind();
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


void FrameBuffer::renderToThis()
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//We tell here which color buffers we want to render. if we have no texture we always render one buffer.

	
}

void FrameBuffer::renderToScreen()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.1, 0.1, 0.1, 0);
}

void FrameBuffer::addColorOutputTexture(std::shared_ptr<Texture> texture)
{
	//we bind the frame buffer.
	bind();
	//outputTexture = texture;
	texture->bind();
	//We associate a 2D texture to the one we bound.
	//We don't send any data to this texture sinec the data will come from the rendering.

	GLenum textureColorSize = GL_RGB;
	if (isHDR)
	{
		textureColorSize = GL_RGB16F;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, textureColorSize, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	//we send the image to the graphic card
	glTexImage2D(GL_TEXTURE_2D, 0, textureColorSize, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

	glDrawBuffers(colorLayout.size(), &colorLayout[0]);

	//We unbind everything
	glBindTexture(GL_TEXTURE_2D, 0);
	unbind();
}

void FrameBuffer::setDepthTexture(std::shared_ptr<Texture> texture)
{
	GLuint error = glGetError();
	//we bind the frame buffer.
	bind();
	//outputTexture = texture;
	texture->bind();
	//We associate a 2D texture to the one we bound.
	//We don't send any data to this texture since the data will come from the rendering.
	texture->setDimensions(width, height);
	texture->setDataType(GL_UNSIGNED_INT_24_8);
	texture->setFormat(GL_DEPTH_STENCIL);
	//I had a really hard time geting this working since I couldn't find information on the fact that you cannot get this texture working if you don't use this specific type of data on my hardware.
	//It was a definite pain.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//we send the image to the graphic card
	error = glGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glBindTexture(GL_TEXTURE_2D, 0);
	this->depthBufferTexture = texture;
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT , GL_TEXTURE_2D, texture->getId(), 0);
	GLenum myerror = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
	error = glGetError();
	GLenum errorFBO = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	
	unbind();
}

std::shared_ptr<Texture> tetraRender::FrameBuffer::getDepthTexture()
{
	return depthBufferTexture;
}

void tetraRender::FrameBuffer::setHDR(bool hdrVal)
{
	//TODO : update all textures
	if (isHDR != hdrVal)
	{

	}
	isHDR = hdrVal;
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
}

void tetraRender::FrameBuffer::bind(GLenum target)
{
	glBindFramebuffer(target, frameBufferId);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void tetraRender::FrameBuffer::clear(tetraBitField mask)
{
	bind();
	glClear(mask);

}

glm::vec2 tetraRender::FrameBuffer::getSize()
{
	return glm::vec2(width, height);
}

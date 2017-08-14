#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glew/glew.h>


Shader::Shader()
{
}

Shader::Shader(std::string vertex, std::string fragment)
{

	vertex = "shaders/" + vertex;
	fragment = "shaders/" + fragment;
	std::ifstream vertexContent(vertex);
	std::ifstream fragmentContent(fragment);
	sourceVertex = std::string((std::istreambuf_iterator<char>(vertexContent)), std::istreambuf_iterator<char>());
	sourceFragment = std::string((std::istreambuf_iterator<char>(fragmentContent)), std::istreambuf_iterator<char>());
	
	vertexId = glCreateShader(GL_VERTEX_SHADER);
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	//compiling both shaders
	compile();
	program = glCreateProgram();
	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);
	//once they are linked to a program they are deleted
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId); 
	glLinkProgram(program);
	glUseProgram(program);

	verticesNum = 0;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

}


Shader::~Shader()
{
}

unsigned int Shader::getProgram()
{
	return program;
}


//Compiling both shaders, vertex and shader;
void Shader::compile()
{
	compileVertex();
	compileFragment();
}

void Shader::compileVertex()
{
	const char* shadersource = sourceVertex.c_str();
	std::cout << "source: " << sourceVertex << std::endl;;
	glShaderSource(vertexId, 1, &shadersource, NULL);
	glCompileShader(vertexId);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << shadersource;
	}
}

void Shader::compileFragment()
{
	const char* shadersource = sourceFragment.c_str();
	std::cout << "source: " << shadersource << std::endl;;
	glShaderSource(fragmentId, 1, &shadersource, NULL);
	glCompileShader(fragmentId);
	int  success;
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);

	char infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << shadersource;
	}

}

void Shader::setVertex(const GLfloat * vertices, unsigned int size)
{
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	std::cout << "size: " << size << " " << vertices[15] << std::endl;
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

void Shader::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 6); //drawing 6 vertices.
	glDisableVertexAttribArray(0);
}

GLuint Shader::getvertexBuffer()
{
	return vertexbuffer;
}

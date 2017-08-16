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

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

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

void Shader::setVertex(std::vector<GLfloat> vertices, std::vector<int> index)
{
	indexSize = index.size();
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	float vertices2[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(unsigned int), &index[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	//std::cout << "error :" << glGetError() << std::endl;
}

void Shader::draw()
{
	//glDrawArrays(GL_TRIANGLES, 0, verticesNum); //drawing as many vertices as their are: 
	//std::cout << "error :"<<glGetError()<<std::endl;
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);

	//glDeleteVertexArrays(1, &vertexbuffer);
	//glDeleteBuffers(1, &vertexbuffer);

}

GLuint Shader::getvertexBuffer()
{
	return vertexbuffer;
}

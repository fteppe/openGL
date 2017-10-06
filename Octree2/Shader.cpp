#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glew/glew.h>


Shader::Shader()// : diffuse()
{
}

Shader::Shader(std::string vertex, std::string fragment) : Shader()
{

	vertex = "shaders/" + vertex;
	fragment = "shaders/" + fragment;
	std::ifstream vertexContent(vertex);
	std::ifstream fragmentContent(fragment);


	vertexId = glCreateShader(GL_VERTEX_SHADER);
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	//compiling both shaders
	compileShader(vertexId, vertex);
	compileShader(fragmentId, fragment);
	//compile();
	program = glCreateProgram();
	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);
	//once they are linked to a program they are deleted
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	glLinkProgram(program);
	glUseProgram(program);

	GLuint* arrayAdress = &VertexArrayID;
	glGenVertexArrays(1, arrayAdress);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
}


Shader::~Shader() 
{
	
}

unsigned int Shader::getProgram()
{
	return program;
}

void Shader::setVertex(std::vector<GLfloat> attributes, std::vector<std::vector<unsigned long long int>> attributesData, std::vector<int> faces)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//we fill the buffer that contains the indexes.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(GLfloat), &attributes[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	for (int i = 0; i < attributesData.size(); i++)
	{
		//all the argments are one after the other, so there is no stride, but we set the offset
		glVertexAttribPointer(i, attributesData[i][1], GL_FLOAT, GL_FALSE, 0, (void*)(offset));
		//we enable the attrib array, meaning i is the number of attribute for one vertex
		glEnableVertexAttribArray(i);
		//we offset by the number of attributes we added 
		offset += attributesData[i][0] * sizeof(GLfloat);
	}
}



void Shader::compileShader(GLuint shader, std::string shaderPath)
{
	std::ifstream shaderSource(shaderPath);
	std::string source = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());

	const char* shaderChar = source.c_str();
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << source;
	}
}



void Shader::setDiffuse(Texture tex)
{
	diffuse = tex;
}



void Shader::draw(std::vector<GLfloat> attributes, std::vector<std::vector<unsigned long long int>> attributesData, std::vector<int> faces)
{
	int errors = glGetError();
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//we fill the buffer that contains the indexes.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(GLfloat), &attributes[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	for (int i = 0; i < attributesData.size(); i++)
	{
		//all the argments are one after the other, so there is no stride, but we set the offset
		glVertexAttribPointer(i, attributesData[i][1], GL_FLOAT, GL_FALSE, 0, (void*)(offset));
		//we enable the attrib array, meaning i is the number of attribute for one vertex
		glEnableVertexAttribArray(i);
		//we offset by the number of attributes we added 
		offset += attributesData[i][0] * sizeof(GLfloat);
	}
	glUseProgram(program);
	if (diffuse.textureLoaded())
	{
		diffuse.applyTexture(program, "diffuse");
	}

	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, (void*)0);
	errors = glGetError();
	//glDeleteVertexArrays(1, &VertexArrayID);
	//glDeleteBuffers(1, &vertexbuffer);
}

GLuint Shader::getvertexBuffer()
{
	return vertexbuffer;
}

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

/*
*@param vertices a 2D array of series of n doubles, each n double representing a vertex. 1st line vertex coord and other lines vertex attributes
*@param index indexes of the vertices to build the triangles (read it in sets of 3). ex: 1 2 3 1 4 5 2 3 5...
*@param nbData for each row. So we can have different vector sizes for each attributes
*/
void Shader::setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData)
{
	indexSize = index.size();
	int arraySize = 0;
	std::vector<GLfloat> flatVert;
	//we flatten the 2D array -> 1D array. So we can have all the vertex and their attributes back to back VVVVVVAAAAAABBBBBBB etc...
	for (int i = 0; i < vertices.size(); i++)
	{
		flatVert.insert(flatVert.end(), vertices[i].begin(), vertices[i].end());
	}
	//we fill the buffer that contains the indexes.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(unsigned int), &index[0], GL_STATIC_DRAW);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER,  flatVert.size() * sizeof(GLfloat), &flatVert[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		//all the argments are one after the other, so there is no stride, but we set the offset
		glVertexAttribPointer(i, nbData[i], GL_FLOAT, GL_FALSE, 0, (void*)(offset));
		//we enable the attrib array, meaning i is the number of attribute for one vertex
		glEnableVertexAttribArray(i);
		//std::cout << "offset :" << offset/sizeof(GLfloat) <<" nbData "<< nbData[i];
		offset += vertices[i].size() * sizeof(GLfloat);
	}

}

void Shader::setDiffuse(Texture tex)
{
	diffuse = tex;
}

void Shader::draw()
{

	diffuse.applyTexture(program, "diffuse");
	//glDrawArrays(GL_TRIANGLES, 0, verticesNum); //drawing as many vertices as their are: 
	//std::cout << "error :"<<glGetError()<<std::endl;
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);

	//glDeleteVertexArrays(1, &VertexArrayID);
	//glDeleteBuffers(1, &vertexbuffer);

}

GLuint Shader::getvertexBuffer()
{
	return vertexbuffer;
}

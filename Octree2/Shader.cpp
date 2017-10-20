#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glew/glew.h>
#include <glm\gtc\type_ptr.hpp>


Shader::Shader()
{
}

Shader::Shader(std::string vertex, std::string fragment)
{

	vertex = "shaders/" + vertex;
	fragment = "shaders/" + fragment;
	std::string lightCalc = "shaders/lightCalc.frag";

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint lumDiffuseCalc = glCreateShader(GL_FRAGMENT_SHADER);
	//compiling both shaders
	compileShader(vertexId, vertex);
	compileShader(fragmentId, fragment);
	compileShader(lumDiffuseCalc, lightCalc);

	program = glCreateProgram();
	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);
	glAttachShader(program, lumDiffuseCalc);
	//once they are linked to a program they are deleted
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	glDeleteShader(lumDiffuseCalc);
	glLinkProgram(program);
	glUseProgram(program);

	//vertex array: used to make the vertex array attributions. To tell when each vertex data starts.
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	//Buffer that is used to send all the vertex data.
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//Buffer that is used to set all the indice of the triangles, from the array buffer.
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
}

//Shader::Shader(const Shader & shader) : program(shader.program), texChannels(shader.texChannels), indexSize(shader.indexSize)
//{
//	//vertex array: used to make the vertex array attributions. To tell when each vertex data starts.
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);
//
//	// Generate 1 buffer, put the resulting identifier in vertexbuffer
//	//Buffer that is used to send all the vertex data.
//	glGenBuffers(1, &vertexbuffer);
//	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	//Buffer that is used to set all the indice of the triangles, from the array buffer.
//	glGenBuffers(1, &elementbuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//}


Shader::~Shader()
{
	//Creates problems if differents objects with different life expectancies have the same shader. This create a memory leak, but will keep it thatway for now.
	//TODO: handle this memory leak. Might need to forbid shader sharing between objects.
	//glDeleteBuffers(1,&elementbuffer);
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteVertexArrays(1, &VertexArrayID);

}

unsigned int Shader::getProgram()
{
	return program;
}




/*
*@param vertices a 2D array of series of n doubles, each n double representing a vertex. 1st line vertex coord and other lines vertex attributes
*@param index indexes of the vertices to build the triangles (read it in sets of 3). ex: 1 2 3 1 4 5 2 3 5...
*@param nbData for each row. So we can have different vector sizes for each attributes
*/
void Shader::setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData)
{
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//We tel openGL that all these shader operation will be done one this shader's program.
	glUseProgram(program);
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
	//We specify which buffer we use.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER,  flatVert.size() * sizeof(GLfloat), &flatVert[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;

	glBindVertexArray(VertexArrayID);
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
	//if there is no diffuse texture, we extend the size of the array
	texChannels["diffuse"] = tex;
}

void Shader::setProgramInformation(Scene const& scene, Solid const& object)
{
	Camera cam = scene.getCam();
	Light light = scene.getLight();
	//We get the light data;
	std::vector<float> lightData(light.getDataArray());
	//we get the camera space and calulculate the projection that will be done to all the vertices
	glm::mat4 cameraSpace = cam.getProjection();
	glm::mat4 objectSpace = object.getObjectSpace();
	glm::mat4 worldSpace = cameraSpace * objectSpace;
	//the projection matrix sent to the shader
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, glm::value_ptr(worldSpace));
	//the objectspace that can be used to calculate lights or the posiiton of a vertex to a point. We send it to the shader.
	glUniformMatrix4fv(glGetUniformLocation(program, "objectSpace"), 1, false, glm::value_ptr(objectSpace));
	//we send the light data to the shader, for now we can handle only one light
	glUniform1fv(glGetUniformLocation(program, "light"), lightData.size(), &lightData[0]);

}

void Shader::sendTexChannels()
{
	//iterating through the map of channels.
	for (std::map<std::string, Texture>::iterator it = texChannels.begin(); it != texChannels.end(); it++)
	{
		//we send to the program the channel.
		it->second.applyTexture(program, it->first);
	}
}

void Shader::draw()
{

	sendTexChannels();
	glUseProgram(program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBindVertexArray(VertexArrayID);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);

}

GLuint Shader::getvertexBuffer()
{
	return vertexbuffer;
}

void Shader::applyToNewObject()
{
	//	//vertex array: used to make the vertex array attributions. To tell when each vertex data starts.
	//	glGenVertexArrays(1, &VertexArrayID);
	//	glBindVertexArray(VertexArrayID);
	//
	//	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	//	//Buffer that is used to send all the vertex data.
	//	glGenBuffers(1, &vertexbuffer);
	//	// The following commands will talk about our 'vertexbuffer' buffer
	//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//	//Buffer that is used to set all the indice of the triangles, from the array buffer.
	//	glGenBuffers(1, &elementbuffer);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
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
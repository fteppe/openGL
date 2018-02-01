#include "stdafx.h"
#include "VertexBufferObject.h"


VertexBufferObject::VertexBufferObject()
{
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


VertexBufferObject::~VertexBufferObject()
{
	//For now creates issues because during a copy it can be destroyed by the compiler. Which is not what I want. So will need some time to figure it out.
	glDeleteBuffers(1,&elementbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void VertexBufferObject::setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData)
{
	//We bind these buffers because they are the one we are gooing to do these operations on.
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	indexSize = index.size();
	int arraySize = 0;
	std::vector<GLfloat> flatVert;
	//we flatten the 2D array -> 1D array. So we can have all the vertex and their attributes back to back AAAAAABBBBBBBBCCCCCC etc...
	for (int i = 0; i < vertices.size(); i++)
	{
		flatVert.insert(flatVert.end(), vertices[i].begin(), vertices[i].end());
	}
	//we fill the buffer that contains the indexes.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);
	//We specify which buffer we use.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, flatVert.size() * sizeof(GLfloat), &flatVert[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	glBindVertexArray(VertexArrayID);
	//This allows us to tell openGL how to separate all of our datas.
	for (int i = 0; i < vertices.size(); i++)
	{
		//we enable the attrib array, meaning i is the number of attribute for one vertex, if there is vertexPos + normal + uv 
		//i=0 => vertex; i=1 => normal ... 
		glEnableVertexAttribArray(i);
		//all the argments are one after the other, so there is no stride, but we set the offset
		//nbData[i] tells openGL the size of each type of data one (vertex = 3 doubles, UV = 2 doubles)
		//the stride variable is set to 0 because each element is back to back VVVVVVVVVVVVVVVVNNNNNNNNNUUUUUUUU ....
		glVertexAttribPointer(i, nbData[i], GL_FLOAT, GL_FALSE, 0, (void*)(offset));
		//The offset is used to position openGL in the vertex buffer object array. It tells openGL when the vertexAttributes starts.
		//since each attributes is packed together, the offset is each time all the attributes.
		offset += vertices[i].size() * sizeof(GLfloat);
	}
}

void VertexBufferObject::sendVertexToShader(const Shader& shader)
{
	int error;
	error = glGetError();
	GLuint program = shader.getProgram();
	const GLubyte* errorString = glewGetErrorString(error);
	//We specify which program will use this VBO
	glUseProgram(program);

	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);	

	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);
	error = glGetError();
	

}

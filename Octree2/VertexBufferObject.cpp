#include "stdafx.h"
#include <iostream>
#include "VertexBufferObject.h"
#include "Polygon.h"


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
	std::cout << "Construction VBO "<< vertexbuffer << std::endl;
}

VertexBufferObject::VertexBufferObject(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn) : VertexBufferObject()
{
	vertices = verticesIn;
	std::vector<std::vector<int>> triangles;
	int offset = 0;
	for (int i = 0; i < indexIn.size(); i++)
	{
		if (indexIn[i].size() > 3)
		{
			//std::cout << __FILE__ << "::" << __LINE__ << "ERROR :: not a triangle" << std::endl;
			triangles = perso::Polygon::triangleSplittingIndex(vertices, indexIn[i], offset);
			index.insert(index.end(), triangles.begin(), triangles.end());
		}
		else
		{
			index.push_back(indexIn[i]);
		}
	}

}


VertexBufferObject::~VertexBufferObject()
{
	//For now creates issues because during a copy it can be destroyed by the compiler. Which is not what I want. So will need some time to figure it out.
	glDeleteBuffers(1,&elementbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	std::cout << "destruction VBO "<< vertexbuffer << std::endl;
}

void VertexBufferObject::setUVs(std::vector<glm::vec3> UVin)
{
	UVs = UVin;
	updateObjectAttributes();
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

void VertexBufferObject::drawObject(const Shader& shader)
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

void VertexBufferObject::setNormals(std::vector<glm::vec3> normalIn)
{
	normals = normalIn;
	updateObjectAttributes();
}

void VertexBufferObject::updateObjectAttributes()
{
	//if we add a new component to our object, we need to update the information on the GC
	std::vector<GLfloat> vertArray;
	std::vector<GLfloat> normalArray;
	std::vector<GLfloat> UVArray;
	std::vector<GLfloat> vec;
	std::vector<int> flatIndex;
	//We should be able to do that before instead of doing it every frame.
	//We flatten all our arrays;
	for (int i = 0; i < vertices.size(); i++)
	{
		vec = std::vector<GLfloat>({ vertices[i].x, vertices[i].y, vertices[i].z });
		vertArray.insert(vertArray.end(), vec.begin(), vec.end());

		if (normals.size())
		{
			std::vector<GLfloat>normal({ normals[i].x,normals[i].y,normals[i].z });
			normalArray.insert(normalArray.end(), normal.begin(), normal.end());
		}
		if (UVs.size())
		{
			std::vector<GLfloat>uv({ UVs[i].x,UVs[i].y });
			UVArray.insert(UVArray.end(), uv.begin(), uv.end());
		}
	}
	for (int i = 0; i < index.size(); i++)
	{

		for (int j = 0; j < index[i].size(); j++)
		{
			flatIndex.push_back(index[i][j]);
		}
	}

	std::vector<std::vector<GLfloat>> vertexData;
	//We add the vertex coord to the vertex Data array.
	vertexData.push_back(vertArray);
	//The 3D vertex Coordinate has 2 components
	std::vector<int> attributeSize({ 3 });
	if (normalArray.size())
	{
		//We add the datas of the normals
		vertexData.push_back(normalArray);
		//Each normal has 3 components
		attributeSize.push_back(3);
	}
	if (UVArray.size())
	{
		//We add the UVcoordinates data
		vertexData.push_back(UVArray);
		//a UV coordinate has 2 parts
		attributeSize.push_back(2);
	}
	setVertex(vertexData, flatIndex, attributeSize);
}

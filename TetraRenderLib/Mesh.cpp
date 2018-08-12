 #include "stdafx.h"
#include <iostream>
#include "Mesh.h"
#include "Polygon.h"

using namespace tetraRender;

Mesh::Mesh()
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

Mesh::Mesh(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn) : Mesh()
{
	setVertexAndIndex(verticesIn, indexIn);
}


Mesh::~Mesh()
{
	//For now creates issues because during a copy it can be destroyed by the compiler. Which is not what I want. So will need some time to figure it out.
	glDeleteBuffers(1,&elementbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	std::cout << "destruction VBO "<< vertexbuffer << std::endl;
}

void Mesh::setUVs(std::vector<glm::vec2> UVin)
{
	UVs = UVin;
	upToDate = false;
}

void Mesh::setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData)
{
	//We bind these buffers because they are the one we are gooing to do these operations on.
	glBindVertexArray(VertexArrayID);
	//We bind this element buffer to the VAO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//This is the size of the index but once it's flat.
	nbVertices = index.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);
	int arraySize = 0;
	std::vector<GLfloat> flatVert;
	//we flatten the 2D array -> 1D array. So we can have all the vertex and their attributes back to back AAAAAABBBBBBBBCCCCCC etc...
	for (unsigned i = 0; i < vertices.size(); i++)
	{
		flatVert.insert(flatVert.end(), vertices[i].begin(), vertices[i].end());
	}
	//we fill the buffer that contains the indexes.
	
	//We specify which buffer we use.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, flatVert.size() * sizeof(GLfloat), &flatVert[0], GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	glBindVertexArray(VertexArrayID);
	//This allows us to tell openGL how to separate all of our datas.
	for (unsigned i = 0; i < vertices.size(); i++)
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

void tetraRender::Mesh::setVertex(GLfloat * verticesData, std::vector<int> attributesSize, std::vector<int> flatFaces)
{
	std::cout << "updating GPU data of " << VertexArrayID << std::endl;
	//We bind these buffers because they are the one we are gooing to do these operations on.
	glBindVertexArray(VertexArrayID);
	//We bind this element buffer to the VAO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//This is the size of the index but once it's flat.
	nbVertices = flatFaces.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, flatFaces.size() * sizeof(int), &flatFaces[0], GL_STATIC_DRAW);
	int arraySize = 0;
	int totalFloatNumber = 0;
	int vertexNumber = vertices.size();
	for (int attributeSize : attributesSize)
	{
		totalFloatNumber += attributeSize * vertexNumber;
	}

	//We specify which buffer we use.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, totalFloatNumber * sizeof(GLfloat), verticesData, GL_STATIC_DRAW);
	//We are going to set each vertex data
	int offset = 0;
	glBindVertexArray(VertexArrayID);
	//This allows us to tell openGL how to separate all of our datas.
	for (unsigned i = 0; i < attributesSize.size(); i++)
	{
		//we enable the attrib array, meaning i is the number of attribute for one vertex, if there is vertexPos + normal + uv 
		//i=0 => vertex; i=1 => normal ... 
		glEnableVertexAttribArray(i);
		//all the argments are one after the other, so there is no stride, but we set the offset
		//nbData[i] tells openGL the size of each type of data one (vertex = 3 doubles, UV = 2 doubles)
		//the stride variable is set to 0 because each element is back to back VVVVVVVVVVVVVVVVNNNNNNNNNUUUUUUUU ....
		glVertexAttribPointer(i, attributesSize[i], GL_FLOAT, GL_FALSE, 0, (void*)(offset));
		//The offset is used to position openGL in the vertex buffer object array. It tells openGL when the vertexAttributes starts.
		//since each attributes is packed together, the offset is each time all the attributes.
		offset += vertices.size() * attributesSize[i] * sizeof(GLfloat);
	}
}

GLfloat * tetraRender::Mesh::createDataArray()
{
	unsigned ArraySize = vertices.size() * 3;
	ArraySize += UVs.size() * 2;
	ArraySize += normals.size() * 3;
	ArraySize += tangents.size() * 3;
	ArraySize += biTangents.size() * 3;

	GLfloat* datas = new GLfloat[ArraySize];
	unsigned int offset = 0;
	attributesSize.clear();
	if (vertices.size())
	{
		offset = updateDataArray(datas, offset, vertices);
		attributesSize.push_back(3);
	}
	if (normals.size())
	{
		offset = updateDataArray(datas, offset, normals);
		attributesSize.push_back(3);

	}
	if (UVs.size())
	{
		offset = updateDataArray(datas, offset, UVs);
		attributesSize.push_back(2);

	}
	if (tangents.size())
	{
		offset = updateDataArray(datas, offset, tangents);
		attributesSize.push_back(3);

	}
	if (biTangents.size())
	{
		offset = updateDataArray(datas, offset, biTangents);
		attributesSize.push_back(3);

	}
	return datas;
}

void tetraRender::Mesh::asyncAttributesUpdate()
{
	//If we are waiting for some data.
	if (dataFuture.valid())
	{
		//If the data has been updated, then we will send it to the GPU
		if (dataFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			GLfloat* data = dataFuture.get();
			std::vector<int> flatIndex;
			for (int i = 0; i < index.size(); i++)
			{

				for (int j = 0; j < index[i].size(); j++)
				{
					flatIndex.push_back(index[i][j]);
				}
			}
			setVertex(data, attributesSize, flatIndex);
			delete data;
			upToDate = true;
		}
	}
}

int tetraRender::Mesh::updateDataArray(GLfloat * dataArray, int offset, std::vector<glm::vec3>& datatoAdd)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		dataArray[offset + i * 3] = datatoAdd[i].x;
		dataArray[offset + i * 3 + 1] = datatoAdd[i].y;
		dataArray[offset + i * 3 + 2] = datatoAdd[i].z;
	}
	offset += datatoAdd.size() * 3;

	return offset;
}

int tetraRender::Mesh::updateDataArray(GLfloat * dataArray, int offset, std::vector<glm::vec2>& datatoAdd)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		dataArray[offset + i * 2] = datatoAdd[i].x;
		dataArray[offset + i * 2 + 1] = datatoAdd[i].y;
	}
	offset += datatoAdd.size() * 2;
	return offset;
}

void Mesh::setFilePath(std::pair<std::string, std::string> filePath)
{
	this->filePath = filePath;
}

std::pair<std::string, std::string> Mesh::getFilePath() const
{
	return filePath;
}

void Mesh::findTangents()
{
	//to find the tangent we need UV coordinates
	if (UVs.size() > 0)
	{
		//there as many tangents as there are vertices;
		tangents = std::vector<glm::vec3>(vertices.size());
		biTangents = std::vector<glm::vec3>(vertices.size());

		for (auto triange : index)
		{
			//we need to find the tangent for each point, and we need 
			unsigned int P0 = triange[0];
			unsigned int P1 = triange[1];
			unsigned int P2 = triange[2];
			//We need 3 point to find the tanfgent and bi-tangent, since we need to to it for each point 
			//we call the same function but changin the "origin" point
			computeTangent(P0, P1, P2);
			computeTangent(P1, P2, P0);
			computeTangent(P2, P1, P0);
		}
	}
	upToDate = false;

}

void Mesh::drawObject(const Shader& shader)
{
	asyncAttributesUpdate();
	int error;
	//error = glGetError();
	GLuint program = shader.getProgram();
	//const GLubyte* errorString = glewGetErrorString(error);
	//We specify which program will use this VBO
	glUseProgram(program);

	glBindVertexArray(VertexArrayID);

	glDrawElements(GL_TRIANGLES, nbVertices, GL_UNSIGNED_INT, (void*)0);

}

void tetraRender::Mesh::setVertexAndIndex(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn)
{
	vertices = verticesIn;
	std::vector<std::vector<int>> triangles;
	int offset = 0;
	for (int i = 0; i < indexIn.size(); i++)
	{
		if (indexIn[i].size() > 3)
		{
			//std::cout << __FILE__ << "::" << __LINE__ << "ERROR :: not a triangle" << std::endl;
			triangles = tetraRender::Polygon::triangleSplittingIndex(vertices, indexIn[i], offset);
			index.insert(index.end(), triangles.begin(), triangles.end());
		}
		else
		{
			index.push_back(indexIn[i]);
		}
	}
}

void Mesh::setNormals(std::vector<glm::vec3> normalIn)
{
	normals = normalIn;
	upToDate = false;
}

void Mesh::updateObjectAttributes()
{
	//if we add a new component to our object, we need to update the information on the GC
	//We also look for the tangents;
	findTangents();
	/*
	std::vector<GLfloat> vertArray;
	std::vector<GLfloat> normalArray;
	std::vector<GLfloat> UVArray;
	std::vector<GLfloat> tangentsArray;
	std::vector<GLfloat> biTangentsArray;
	std::vector<GLfloat> vec;
	std::vector<int> flatIndex;
	//We should be able to do that before instead of doing it every frame.
	//We flatten all our arrays;
	for (unsigned i = 0; i < vertices.size(); i++)
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
		if (tangents.size())
		{
			std::vector<GLfloat>tan({ tangents[i].x,tangents[i].y, tangents[i].z });
			tangentsArray.insert(tangentsArray.end(), tan.begin(), tan.end());
		}
		if (biTangents.size())
		{
			std::vector<GLfloat>tan({ biTangents[i].x,biTangents[i].y, biTangents[i].z });
			biTangentsArray.insert(biTangentsArray.end(), tan.begin(), tan.end());
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
	if (tangentsArray.size() > 0)
	{
		//if we have tangents we als ohave bitangents
		
		vertexData.push_back(tangentsArray);
		attributeSize.push_back(3);
		vertexData.push_back(biTangentsArray);
		attributeSize.push_back(3);
	}
	*/
	
	std::vector<int> flatIndex;
	for (int i = 0; i < index.size(); i++)
	{

		for (int j = 0; j < index[i].size(); j++)
		{
			flatIndex.push_back(index[i][j]);
		}
	}
	GLfloat* vertexData = createDataArray();
	setVertex(vertexData,  attributesSize, flatIndex);
	delete vertexData;
	upToDate = true;
}

void tetraRender::Mesh::setDataFuture(std::future<GLfloat*> future)
{
	dataFuture = std::move(future);
}

void Mesh::computeTangent(unsigned int P0, unsigned int P1, unsigned int P2)
{
	//TODO : correct the use case where the normal is not normal to the face.
	//We need to get a tangent in the right direction but that is also normal to the normal
	glm::vec3 point0 = vertices[P0];
	glm::vec3 point1 = vertices[P1];
	glm::vec3 point2 = vertices[P2];
	//First we check that the tangent and bitangent haven't been computed already
	if (tangents[P0] == glm::vec3(0, 0, 0) && biTangents[P0] == glm::vec3(0, 0, 0))
	{
		//those points are in object space
		glm::vec3 E1 = point1 - point0;
		glm::vec3 E2 = point2 - point0;

		glm::vec2 delta1 = UVs[P1] - UVs[P0];
		glm::vec2 delta2 = UVs[P2] - UVs[P0];

		float a = delta1.y * delta2.x - delta1.x*delta2.y;

		glm::vec3 tangent = glm::vec3(
			(-delta2.y * E1.x + delta1.y * E2.x) / a,
			(-delta2.y * E1.y + delta1.y * E2.y) / a,
			(-delta2.y * E1.z + delta1.y * E2.z) / a);

		glm::vec3 biTangent = glm::vec3(
			(delta2.x*E1.x - delta1.x*E2.x) / a,
			(delta2.x*E1.y - delta1.x*E2.y) / a,
			(delta2.x*E1.z - delta1.x*E2.z) / a
		);

		tangents[P0] = tangent;
		biTangents[P0] = biTangent;
	}
}

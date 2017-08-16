#include "stdafx.h"
#include "Solid.h"
#include <sstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glew\glew.h>
Solid::Solid() : objectSpace() , polygons(), triangulated(false)
{
}

Solid::Solid(std::vector<perso::Polygon> poly) : objectSpace(glm::mat4())
{

}

Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn):objectSpace(glm::mat4()), shader("transform.ver", "col.frag")
{
	vertices = verticesIn;
	std::vector<std::vector<int>> triangles;
	int offset = 0;
	for (int i = 0; i < indexIn.size(); i++)
	{
		triangles = perso::Polygon::triangleSplittingIndex(vertices,indexIn[i],offset);
		index.insert(index.end(), triangles.begin(), triangles.end());

	}
}


Solid::~Solid()
{
}

void Solid::draw()
{
	std::vector<GLfloat> vertArray;
	std::vector<GLfloat> vec;
	std::vector<int> flatIndex;
	for (int i = 0; i < vertices.size();i++)
	{
		vec = std::vector<GLfloat>({ vertices[i].x, vertices[i].y, vertices[i].z });
		vertArray.insert(vertArray.end(), vec.begin(), vec.end());
	}
	for (int i = 0; i < index.size(); i++)
	{

		for (int j = 0; j < index[j].size(); j++)
		{
			flatIndex.push_back(index[i][j]);
		}
	}

	
	shader.setVertex(vertArray, flatIndex);

	//glm::mat4 mvp = glm::perspective(glm::radians(45.0f), 800 / 600, 0.1f, 100.0f);
	unsigned int program = shader.getProgram();

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, glm::value_ptr(objectSpace));

	shader.draw();
}

std::string Solid::description()
{
	std::stringstream ss;
	ss << "polygons :" << std::endl;
	for (int i = 0; i < index.size(); i++)
	{

		for (int j = 0; j < index[j].size(); j++)
		{
			ss << index[i][j]<<" ";
		}
		ss << std::endl;
	}
	return ss.str();
}

void Solid::setObjectSpace(glm::mat4 transfo)
{
	objectSpace = transfo;
}

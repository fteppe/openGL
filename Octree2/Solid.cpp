#include "stdafx.h"
#include "Solid.h"
#include "Scene.h"

#include <sstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glew\glew.h>

Solid::Solid() : objectSpace(), triangulated(false)
{
}


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn):objectSpace(glm::mat4()), shader("transform.ver", "col.frag")
{
	vertices = verticesIn;
	std::vector<std::vector<int>> triangles;
	int offset = 0;
	for (int i = 0; i < indexIn.size(); i++)
	{
		if (indexIn[i].size() > 3)
		{
			//std::cout << "Solid.cpp >> needs splitting";
			triangles = perso::Polygon::triangleSplittingIndex(vertices, indexIn[i], offset);
			index.insert(index.end(), triangles.begin(), triangles.end());
		}
		else
		{
			index.push_back(indexIn[i]);
		}
	}
}


Solid::~Solid()
{
}

void Solid::draw(Scene const scene)
{

	std::vector<GLfloat> vertArray;
	std::vector<GLfloat> normalArray;
	std::vector<GLfloat> UVArray;
	std::vector<GLfloat> vec;
	std::vector<int> flatIndex;
	std::vector<GLfloat> col = { 0.1f,0.2f,0.2f };
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
			std::vector<GLfloat>uv({ UVs[i].x,UVs[i].y});
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
	std::vector<int> attributeSize({ 3 });
	std::vector<std::vector<GLfloat>> vertexData;
	vertexData.push_back(vertArray);
	if (normalArray.size())
	{
		vertexData.push_back(normalArray);
		attributeSize.push_back(3);
	}
	if (UVArray.size())
	{
		vertexData.push_back(UVArray);
		attributeSize.push_back(2);
	}

	shader.setVertex(vertexData, flatIndex, attributeSize);
	//shader.setProgramInformation(scene, *this);


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
	ss << "vertices :" << std::endl;
	for (int i = 0; i < vertices.size(); i++)
	{
		ss << vertices[i] << std::endl;
	}
	return ss.str();
}

void Solid::setObjectSpace(glm::mat4 transfo)
{
	objectSpace = transfo;
}

void Solid::setNormals(std::vector<glm::vec3> normalIn)
{
	normals = normalIn;
}

void Solid::setUVs(std::vector<glm::vec3> UVin)
{
	UVs = (UVin);
}

void Solid::setShader(Shader shade)
{
	shader = shade;

}

void Solid::setTexture(Texture tex)
{
	shader.setDiffuse(tex);
}

glm::mat4 Solid::getObjectSpace() const
{
	return objectSpace;
}

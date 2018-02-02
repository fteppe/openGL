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


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn): Solid()
{
	objectSpace = glm::mat4(); 
	//shader = Shader("transform.ver", "col.frag");
	shader_ptr = std::shared_ptr<Shader>(new Shader("transform.ver", "col.frag"));
	VBO_ptr = std::shared_ptr<VertexBufferObject>(new VertexBufferObject);
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
	triangulated = true;
	updateVertexAttributes();

}


Solid::~Solid()
{
	//shader_ptr.reset();
}

void Solid::draw(Scene const& scene)
{


	//shader_ptr->setProgramInformation(scene, *this);

	//VBO.sendVertexToShader(*shader_ptr);
	//VBO_ptr->sendVertexToShader(*shader_ptr);

	material_ptr->apply(VBO_ptr, scene, *this);
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
	updateVertexAttributes();
}

void Solid::setUVs(std::vector<glm::vec3> UVin)
{
	UVs = (UVin);
	updateVertexAttributes();
}


void Solid::setMaterial(std::shared_ptr<Material> const & mat)
{
	material_ptr = mat;
}

glm::mat4 Solid::getObjectSpace() const
{
	return objectSpace;
}

void Solid::updateVertexAttributes()
{
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

	//VBO.setVertex(vertexData, flatIndex, attributeSize);
	VBO_ptr->setVertex(vertexData, flatIndex, attributeSize);
}

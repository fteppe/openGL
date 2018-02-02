#include "stdafx.h"
#include "Solid.h"
#include "Scene.h"

#include <sstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glew\glew.h>

Solid::Solid() : objectSpace(), triangulated(false), shader_ptr(new Shader("transform.ver", "col.frag"))
{

}


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn): Solid()
{

	//shader_ptr = std::shared_ptr<Shader>(new Shader("transform.ver", "col.frag"));
	//VBO_ptr = std::shared_ptr<VertexBufferObject>(new VertexBufferObject);
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

}

Solid::Solid(std::weak_ptr<VertexBufferObject> vbo) : Solid()
{
	VBO_ptr = vbo;
}


Solid::~Solid()
{
}

void Solid::draw(Scene const& scene)
{
	//we make sure the object still exists, if it does we render it.
	if (!VBO_ptr.expired())
	{
		material_ptr->apply(VBO_ptr.lock().get(), scene, *this);
	}
	else
	{
		//std::cout << __FILE__ << "::" << __LINE__ << "expired VBO, cannot render object"<<std::endl;
	}
	
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



void Solid::setMaterial(std::shared_ptr<Material> const & mat)
{
	material_ptr = mat;
}

glm::mat4 Solid::getObjectSpace() const
{
	return objectSpace;
}


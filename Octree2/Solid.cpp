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

	for (int i = 0; i < poly.size(); i++)
	{
		std::cout << poly[i].description()<<std::endl;
		std::vector<perso::Polygon> triangles = poly[i].triangleSplitting();
		//and we add the triangle polygons to the end of the vector of our polygons
		polygons.insert(polygons.end(), triangles.begin(), triangles.end());

	}

	//we add the shader and set the vertices.
	shader = Shader("transform.ver", "col.frag");



}


Solid::~Solid()
{
}

void Solid::draw()
{
	std::vector<GLfloat> vertices;
	unsigned int vertNum = 0;
	//We put all the vertices coordinates in one single array.
	for (int i = 0; i < polygons.size(); i++)
	{
		
		std::vector<GLfloat> tab = polygons[i].getVertArray();
		vertices.insert(vertices.end(), tab.begin(), tab.end());
		vertNum += polygons[i].getVertArraySize();
	}
	const GLfloat* vertArray = &vertices[0];
	shader.setVertex(vertArray, vertNum);

	glm::mat4 mvp = glm::rotate(2.0f,glm::vec3(0.5, 0.5, 0.5));
	mvp = objectSpace;
	//glm::mat4 mvp = glm::perspective(glm::radians(45.0f), 800 / 600, 0.1f, 100.0f);
	unsigned int program = shader.getProgram();

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, glm::value_ptr(mvp));

	shader.draw();
}

std::string Solid::description()
{
	std::stringstream ss;
	ss << "polygons :" << std::endl;
	for (int i=0; i < polygons.size(); i++)
	{
		ss << polygons[i].description() << std::endl;
	}
	return ss.str();
}

void Solid::setObjectSpace(glm::mat4 transfo)
{
	objectSpace = transfo;
}

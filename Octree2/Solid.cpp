#include "stdafx.h"
#include "Solid.h"
#include "Scene.h"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glew/glew.h>

Solid::Solid() : GameObject(), triangulated(false)
{

}


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn): Solid()
{

	//shader_ptr = std::shared_ptr<Shader>(new Shader("transform.ver", "col.frag"));
	//VBO_ptr = std::shared_ptr<VertexBufferObject>(new VertexBufferObject);


}

Solid::Solid(std::weak_ptr<VertexBufferObject> vbo) : Solid()
{
	VBO_ptr = vbo;
}


Solid::~Solid()
{
}

void Solid::draw(Scene& scene)
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
	return "TODO";
}


void Solid::setMaterial(std::shared_ptr<Material> const & mat)
{
	material_ptr = mat;
}




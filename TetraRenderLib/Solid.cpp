#include "stdafx.h"
#include "Solid.h"
#include "Scene.h"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glew/glew.h>

using namespace tetraRender;

Solid::Solid() : GameObject(), triangulated(false)
{
}


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn): Solid()
{

	//shader_ptr = std::shared_ptr<Shader>(new Shader("transform.ver", "col.frag"));
	//VBO_ptr = std::shared_ptr<VertexBufferObject>(new VertexBufferObject);


}

Solid::Solid(std::shared_ptr<Mesh> vbo) : Solid()
{
	mesh_ptr = vbo;
}

tetraRender::Solid::Solid(std::pair<std::string, std::string> objFile)
{
}


Solid::~Solid()
{
}

void Solid::draw(tetraRender::Scene& scene)
{
 	GameObject::draw(scene);
	//we make sure the object still exists, if it does we render it.
	material_ptr->apply(mesh_ptr.get(), scene, *this);

}

void Solid::draw(tetraRender::Scene& scene, std::shared_ptr<Material> mat)
{
	GameObject::draw(scene, mat);
	mat->apply(mesh_ptr.get(), scene, *this);
}

std::string Solid::description()
{
	return "TODO";
}

GameObjectType tetraRender::Solid::getType() const
{
	return GameObjectType::SOLID;
}


void Solid::setMaterial(std::shared_ptr<Material> const & mat)
{
	material_ptr = mat;
}




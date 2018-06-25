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
	parametersContainer.set(typeField, std::string("solid"));

}


Solid::Solid(std::vector<glm::vec3> verticesIn, std::vector<std::vector<int>> indexIn): Solid()
{

	//shader_ptr = std::shared_ptr<Shader>(new Shader("transform.ver", "col.frag"));
	//VBO_ptr = std::shared_ptr<VertexBufferObject>(new VertexBufferObject);


}

Solid::Solid(std::shared_ptr<Mesh> vbo) : Solid()
{
	mesh_ptr = vbo;
	auto vboFilePath = mesh_ptr->getFilePath();
	setName(vboFilePath.first + "::"+vboFilePath.second);
}

GameObject * tetraRender::Solid::getDeepCopy()
{
	GameObject* copy = new Solid(*this);
	//the reason we do a deep copy of the children as well is that if we delete the copy we don't want to also delete the original's children.
	copy->copyChildren();

	return copy;
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
	std::shared_ptr<Material> material = material_ptr;
	GameObject* parent = parentNode;
	while (material == nullptr && parent != nullptr)
	{
		material = parent->getMaterial();
		parent = parent->getParent();
	}
	if (material != nullptr)
	{
		material->apply(mesh_ptr.get(), scene, *this);
	}

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



const Mesh & tetraRender::Solid::getMesh()
{
	return *mesh_ptr;
}


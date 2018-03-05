#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include <glm/matrix.hpp>
#include <memory>

#include "Scene.h"
#include "GameObject.h"

class Solid : public GameObject
{
public:
	Solid();
	Solid(std::vector<glm::vec3> vertices, std::vector<std::vector<int>> index);
	Solid(std::shared_ptr<VertexBufferObject> vbo);
	~Solid();
	void draw(Scene& scene);
	std::string description();
	//void setObjectSpace(glm::mat4 transfo);

	void setMaterial(std::shared_ptr<Material> const& mat);

protected:

	bool triangulated;
	
	std::shared_ptr<VertexBufferObject> VBO_ptr;
};


#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include <glm\matrix.hpp>
#include <memory>

#include "Scene.h"
class Solid
{
public:
	Solid();
	Solid(std::vector<glm::vec3> vertices, std::vector<std::vector<int>> index);
	~Solid();
	void draw(Scene const& scene);
	std::string description();
	void setObjectSpace(glm::mat4 transfo);
	void setNormals(std::vector<glm::vec3> normalIn);
	void setUVs(std::vector<glm::vec3> UVin);
	void setMaterial(std::shared_ptr<Material> const& mat);
	glm::mat4 getObjectSpace() const;
protected:
	//All the attributes of our solid in a single 1D array.
	void updateVertexAttributes();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> UVs;
	//A vector of vector of indices. It defines each face of the solid.
	std::vector<std::vector<int>> index;
	glm::mat4 objectSpace;
	bool triangulated;
	std::shared_ptr<Shader> shader_ptr;
	std::shared_ptr<Material> material_ptr;
	VertexBufferObject VBO;
	std::shared_ptr<VertexBufferObject> VBO_ptr;


};


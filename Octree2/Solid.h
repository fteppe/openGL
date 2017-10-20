#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
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
	void setShader(std::shared_ptr<Shader> const& shade);
	void setTexture(Texture tex);
	glm::mat4 getObjectSpace() const;
protected:


	//All the attributes of our solid in a single 1D array.
	void updateVertexAttributes();
	std::vector<GLfloat> attributesArray;
	std::vector<std::vector<unsigned long long int>> attributesData;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> UVs;
	//1D array of all the faces, in sets of 3: f1f1f1 f2f2f2 f3f3f3....
	std::vector<int> flatFaces;
	std::vector<std::vector<int>> index;
	glm::mat4 objectSpace;
	bool triangulated;
	std::shared_ptr<Shader> shader_ptr;
	VertexBufferObject VBO;


};


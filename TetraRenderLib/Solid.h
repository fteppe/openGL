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

namespace tetraRender
{
	class Solid : public GameObject
	{
	public:
		Solid();
		Solid(std::vector<glm::vec3> vertices, std::vector<std::vector<int>> index);
		Solid(std::shared_ptr<VertexBufferObject> vbo);
		//directly loads the VBO from an obj file.
		Solid(std::pair <std::string, std::string> objFile);
		~Solid();
		void draw(tetraRender::Scene& scene);
		void draw(Scene& scene, std::shared_ptr<Material> mat);
		std::string description();
		virtual GameObjectType getType() const;

		void setMaterial(std::shared_ptr<Material> const& mat);

	protected:

		bool triangulated;
		std::shared_ptr<Material> material_ptr;
		std::shared_ptr<VertexBufferObject> VBO_ptr;
	};


}
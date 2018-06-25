#pragma once
#include <vector>
#include "Polygon.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
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
		Solid(std::shared_ptr<Mesh> vbo);		
		/// <summary>
		/// Gets the deep copy of this solid, it should be noted that the material and the mesh are not copied. So the copy will simply add +1 to the shared_ptr reference counter.
		/// </summary>
		/// <returns>returns a Solid* in a GameObject*</returns>
		virtual GameObject* getDeepCopy();
		//directly loads the VBO from an obj file.
		Solid(std::pair <std::string, std::string> objFile);
		~Solid();
		void draw(tetraRender::Scene& scene);
		void draw(Scene& scene, std::shared_ptr<Material> mat);
		std::string description();
		virtual GameObjectType getType() const;
		
		const Mesh& getMesh();

	protected:

		bool triangulated;
		std::shared_ptr<Mesh> mesh_ptr;
	};


}
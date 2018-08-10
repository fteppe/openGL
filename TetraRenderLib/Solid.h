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
#include "MeshLoader.h"


namespace tetraRender
{
	class MeshLoader;

	class Solid : public GameObject
	{
	public:
		Solid();
		Solid(std::shared_ptr<Mesh> vbo);		

		
		/// <summary>
		/// Initializes a new instance of the <see cref="Solid"/> class.
		/// In the case we load the mesh asynchronously, we don't give it a mesh but a reference to an object that can deliver
		/// It when loading is complete.
		/// </summary>
		/// <param name="loader">The reference to the object that can load the mesh and give it once the mesh file has been read</param>
		/// <param name="meshName">Name of the mesh.</param>
		Solid(std::shared_ptr<MeshLoader> loader, std::pair<std::string, std::string> meshName);

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
		/// <summary>
		/// Asynchronouses the mesh update. While it will give an updated version of the mesh it might not be completed.
		/// it will be up to the mesh to devide if it wants to render itself or not.
		/// </summary>
		/// <returns></returns>
		bool asynchronousMeshUpdate();

		bool triangulated;
		//In the case where we have asynchronous loading of the meshes, we keep a reference to the loader to get the mesh for later.
		std::shared_ptr<MeshLoader> loader;
		//In the case of asynchronous mesh loading, the solid might not have a mesh, therefor it keeps it name so it can query the mesh with it.
		std::pair<std::string, std::string> meshName;
		std::shared_ptr<Mesh> mesh_ptr;
	};


}
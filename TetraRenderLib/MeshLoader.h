#pragma once

#include <memory>
#include <map>
#include <future>
#include <string>

#include "Mesh.h"
#include "waveFrontLoader.h"
#include "ResourceAtlas.h"
#include "tiny_obj_loader.h"
namespace tetraRender
{
	struct objData
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
	};
	class MeshLoader
	{
	public:
		MeshLoader(MeshContainer* meshesIn);
		~MeshLoader();
		
		/// <summary>
		/// Gets the mesh, returns nullptr if the file that mesh comes from isn't done loading.
		/// </summary>
		/// <param name="meshName">Name of the mesh. The left side is the fileName, the rightSide is the name of the mesh</param>
		/// <returns>either a mesh that might not be done loading or a nullptr if the parsing of the file isn't over.</returns>
		std::shared_ptr<Mesh> getMesh(std::pair<std::string, std::string> meshName);

	protected:
		GLfloat * updateMesh(std::shared_ptr<Mesh> mesh, objData* objDataLoaded, unsigned meshId);

		std::map<std::string, std::future<objData>> fileLoadingPromises;
		MeshContainer* meshes;
		
	};
}



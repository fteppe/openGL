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
		MeshLoader(ResourceAtlas& meshesIn);
		~MeshLoader();
		
		/// <summary>
		/// Gets the mesh, returns nullptr if the file that mesh comes from isn't done loading.
		/// </summary>
		/// <param name="meshName">Name of the mesh. The left side is the fileName, the rightSide is the name of the mesh</param>
		/// <returns>either a mesh that might not be done loading or a nullptr if the parsing of the file isn't over.</returns
		std::shared_ptr<Mesh> getMesh(std::pair<std::string, std::string> meshName);

		std::vector<std::shared_ptr<Mesh>> getAllMeshes(std::string fileName);

		/// <summary>
		/// Checks if the file containing the mesh has  started loading. If it hasn't it launches loading tasks
		/// If the loading is over it adds incomplete meshes to the atlas and starts mesh update tasks.
		/// </summary>
		/// <param name="meshName">Name of the mesh.</param>
		/// <returns></returns>
		std::vector<MeshName> checkFileLoadingProgress(std::string fileName);

	protected:
		
		/// <summary>
		/// Updates the mesh.
		/// </summary>
		/// <param name="mesh">a pointer to the mesh we need to update.</param>
		/// <param name="objDataLoaded">The wavefront object data loaded.</param>
		/// <param name="meshId">The mesh identifier. It's to know which mesh in the objData coresponds to the tetraRender::Mesh object</param>
		/// <returns>the data that we need to send to the GPU</returns>
		GLfloat * updateMesh(std::shared_ptr<Mesh> mesh, objData* objDataLoaded, unsigned meshId);


		objData* loadFile(std::string fileName);

		std::vector<MeshName> createMeshUpdateTasks(objData* loaded, std::string fileName);
		//void executeUpdateMeshes()

		std::map<std::string, std::future<objData*>> fileLoadingFutures;
		std::vector<std::future<void>> meshUpdateFutures;
		ResourceAtlas& atlas;
		
	};
}



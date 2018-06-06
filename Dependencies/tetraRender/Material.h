#pragma once
#include "Resource.h"
#include "Shader.h"
#include "Mesh.h"
#include <memory>
#include <map>
#include <string>



namespace tetraRender
{
	class Material : public Resource
	{
	public:
		Material();
		Material(std::shared_ptr<Shader> shader);
		~Material();
		//This allows the user to create a texture channel and bind a texture to it.
		//The user lust always be aware that the channel name and the channel in the shader should match
		//Although it is the shader object that will handle sending this information, so a object inheriting form shader could redefine this.
		void setChannel(std::shared_ptr<Texture> text, std::string channel);
		//We send all the relevent informations to the shader that will be necessary for the shader program
		virtual void setProgramInformation(Scene& scene, Solid const& object);
		void apply(Mesh* const& VBO, Scene & scene, Solid const& solid);
		//We get apointer on the shader program associated to this material.
		std::shared_ptr<Shader> getShaderProgram();
		const std::map<std::string, std::shared_ptr<Texture>> getChannels();

	private:
		
		/// <summary>
		/// Fills the parameter container, we query openGL and the shader once it has compiled to get all the uniform variables
		/// that start with pu_ for public.
		/// </summary>
		void fillParameterContainer();
		//We have a pointer on a shader because the same shader can be used with different textures.
		//No ownership of the shader, is only an observer.
		std::shared_ptr<Shader> shader_ptr;
		//We need to have shared pointers on the Textures, because they contain dynamic memory, so we need to make sure that we delete them only once.
		//no ownership of the textures is only an observer.
		std::map<std::string, std::shared_ptr<Texture>> textures;
	};

}


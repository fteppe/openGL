#include "stdafx.h"
#include "Material.h"
#include <iostream>

using namespace tetraRender;

Material::Material()
{
}
Material::Material(std::shared_ptr<Shader> shader) : Material()
{
	shader_ptr = shader;
	fillParameterContainer();
}

Material::~Material()
{
}

void Material::setChannel( std::shared_ptr<Texture> text, std::string channel )
{
	textures[channel] = text;
}

void Material::setProgramInformation(Scene & scene, Solid const & object)
{
	shader_ptr->setProgramInformation(scene, object);
	shader_ptr->sendTexChannels(textures);
}

void Material::apply(Mesh* const& VBO, Scene & scene, Solid const& solid)
{
	shader_ptr->setProgramInformation(scene, solid);
	shader_ptr->sendTexChannels(textures);
	VBO->drawObject(*shader_ptr);
	shader_ptr->resetTextureUnitCount();
	//We take all the parameters of that material and send it to the shader.
	//This is great to have custom public variables and to be able to change them on the fly.
	shader_ptr->use();
	for (auto param : parametersContainer.getParameters())
	{
		if (param.second == ParameterType::VEC3)
		{
			shader_ptr->sendVec3(param.first, parametersContainer.getVec3(param.first));
		}
	}
}

std::shared_ptr<Shader> Material::getShaderProgram()
{
	return shader_ptr;
}

void tetraRender::Material::fillParameterContainer()
{
	GLint count;
	glGetProgramiv(shader_ptr->getProgram(), GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; i++)
	{
		std::string uniformName;
		char* name = new char[GL_ACTIVE_UNIFORM_MAX_LENGTH];
		GLsizei length;
		int size;
		GLenum type;
		glGetActiveUniform(shader_ptr->getProgram(), i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length, &size, &type, name);
		uniformName = std::string(name);
		uniformName.shrink_to_fit();
		if (uniformName.substr(0, 3) == "pu_")
		{
			if (type == GL_FLOAT_VEC3)
			{
				parametersContainer.set(uniformName, glm::vec3(0));
			}
		}
		
	}
	
}


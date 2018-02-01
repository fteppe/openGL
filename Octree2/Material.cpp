#include "stdafx.h"
#include "Material.h"


Material::Material()
{
}
Material::Material(std::shared_ptr<Shader> shader)
{
	shader_ptr = shader;
}

Material::~Material()
{
}

void Material::setChannel( std::shared_ptr<Texture> text, std::string channel )
{
	textures[channel] = text;
}

void Material::setProgramInformation(Scene const & scene, Solid const & object)
{
	shader_ptr->setProgramInformation(scene, object);
	shader_ptr->sendTexChannels(textures);
}

void Material::apply(std::shared_ptr<VertexBufferObject> const& VBO, Scene const& scene, Solid const& solid)
{
	shader_ptr->setProgramInformation(scene, solid);
	shader_ptr->sendTexChannels(textures);
	VBO->sendVertexToShader(*shader_ptr);
}


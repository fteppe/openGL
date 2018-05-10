#include "stdafx.h"
#include "Material.h"

using namespace tetraRender;

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
}

std::shared_ptr<Shader> Material::getShaderProgram()
{
	return shader_ptr;
}


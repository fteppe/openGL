#include "stdafx.h"
#include "Material.h"


Material::Material()
{
}
Material::Material(Shader* shader)
{
	shader_ptr = shader;
}

Material::~Material()
{
}

void Material::setChannel( Texture* text, std::string channel )
{
	textures[channel] = text;
}

void Material::setProgramInformation(Scene & scene, Solid const & object)
{
	shader_ptr->setProgramInformation(scene, object);
	shader_ptr->sendTexChannels(textures);
}

void Material::apply(VertexBufferObject* const& VBO, Scene & scene, Solid const& solid)
{
	shader_ptr->setProgramInformation(scene, solid);
	shader_ptr->sendTexChannels(textures);
	VBO->drawObject(*shader_ptr);
}


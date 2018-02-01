#pragma once
#include "Shader.h"
#include <memory>
#include <map>
#include <string>
class Material
{
public:
	Material();
	~Material();
private:
	std::shared_ptr<Shader> shader_ptr;
	//We need to have shared pointers on the Textures, because they contain dynamic memory, so we need to make sure that we delete them only once.
	std::map<std::string, std::shared_ptr<Texture>> textures_ptr;
};


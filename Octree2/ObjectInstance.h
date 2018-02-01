#pragma once
#include <memory>

#include "Solid.h"
#include "Shader.h"
//This is the instance of a solid. It points to a Program and a Solid. Multiple Object instances can share the same solid, and textures.
class ObjectInstance
{
public:
	ObjectInstance();
	~ObjectInstance();
	
private:
	std::shared_ptr<Solid> solid;
	std::shared_ptr<Shader> shader;
};


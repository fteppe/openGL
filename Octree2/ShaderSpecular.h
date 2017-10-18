#pragma once
#include "Shader.h"
class ShaderSpecular :
	public Shader
{
public:
	ShaderSpecular();
	~ShaderSpecular();
	void setVertex(std::vector<std::vector<GLfloat>> vertices, std::vector<int> index, std::vector<int> nbData);
};


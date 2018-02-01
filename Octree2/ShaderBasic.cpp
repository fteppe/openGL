#include "stdafx.h"
#include "ShaderBasic.h"


ShaderBasic::ShaderBasic() :Shader({ "texture.ver" }, std::vector<std::string>({ "specular.frag","lightCalc.frag"}))
{
}


ShaderBasic::~ShaderBasic()
{
}

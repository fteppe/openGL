#pragma once
#include "Texture.h"

namespace tetraRender
{
	class CubeMap :
		public Texture
	{
	public:
		CubeMap();
		void loadTextures(std::vector<std::string > cubeMapElements);
		~CubeMap();
	};
}



// TetraVisu.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <SDL2/SDL.h>
#include <tetraRender\Camera.h>
#include "WindowBuilder.h"

#undef main
int main(int argc, char *argv[])
{

	std::string sceneName = "scenes/scene.json";
	tetraRender::Camera cam;

	cam;
	tetraRender::Camera* cam_ptr = &cam;
	cam_ptr->setPos(glm::vec3(0, 0, 0));

	WindowBuilder window(sceneName);
	window.draw();

	//ImGui::SFML::Shutdown();
	return 0;
	
}

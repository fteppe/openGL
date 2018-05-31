// TetraVisu.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "WindowBuilder.h"
#include <tetraRender\PreprocessorShader.h>
class WindowBuilder;
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>


int main(int argc, char **argv)
{

	std::string sceneName = "scenes/scene.json";
	if (argc > 1)
	{
		sceneName = argv[1];
	}
	WindowBuilder window(sceneName);

	window.draw();


	//std::cout << "starting application" << std::endl;
	///*
	//WINDOW OPENGL INIT
	//*/
	//const std::string title = "openGL";
	//sf::ContextSettings settings;
	//settings.depthBits = 24;
	//settings.stencilBits = 8;
	//settings.antialiasingLevel = 2; // Optional
	//								// Request OpenGL version 3.2 (optional but recommended)
	//settings.majorVersion = 4;
	//settings.minorVersion = 3;
	//settings.attributeFlags = sf::ContextSettings::Core;

	//unsigned width = WIDTH;
	//unsigned height = HEIGHT;


	//sf::RenderWindow window(sf::VideoMode(width, height), "Tetra Engine", sf::Style::Default);
	//window.setFramerateLimit(60);

	//window.setActive();

	//glewExperimental = GL_TRUE;
	//glewInit();
	//glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//ImGui::CreateContext();
	//ImGui::SFML::Init(window);

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);
	//GLuint i;
	//glCreateBuffers(1, &i);

	//sf::Clock deltaClock;
	//while (window.isOpen()) {
	//	sf::Event event;
	//	while (window.pollEvent(event)) {
	//		ImGui::SFML::ProcessEvent(event);

	//		if (event.type == sf::Event::Closed) {
	//			window.close();
	//		}
	//	}

	//	ImGui::SFML::Update(window, deltaClock.restart());


	//	ImGui::Begin("Hello, world!");
	//	ImGui::Button("Look at this pretty button");
	//	ImGui::End();

	//	window.clear();
	//	window.draw(shape);
	//	window.pushGLStates();
	//	ImGui::SFML::Render(window);
	//	window.popGLStates();
	//	window.display();
	//}

	//ImGui::SFML::Shutdown();


		return 0;
	
}

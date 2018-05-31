#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>

#include <iostream>
#include <iomanip>

#include <crtdbg.h>
#include <tetraRender\Scene.h>
#include <tetraRender\EventHandler.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "imgui.h"
#include "imgui-sfml.h"

//This will build a window using open GL and stuff, this is a way to unclutter the main.
WindowBuilder::WindowBuilder()
{
	std::cout << "starting application" << std::endl;
	/*
	WINDOW OPENGL INIT
	*/
	const std::string title = "openGL";
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2; // Optional
									// Request OpenGL version 3.2 (optional but recommended)
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	unsigned width = WIDTH;
	unsigned height = HEIGHT;

	
	window.create(sf::VideoMode(width, height), "Tetra Engine", sf::Style::Default);
	window.setFramerateLimit(60);

	window.setActive();

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	ImGui::SFML::Init(window);


	
	//apparently an old implementation bug tends to raise an error on startup. We call geterror to remove it.
	glGetError();

	
	
	//std::cout << glGetString(GL_VERSION) << std::endl;
	


}

WindowBuilder::WindowBuilder(std::string sceneFile) : WindowBuilder()
{
	tetraRender::Camera cam(600.0f, 800.0f, 0.75f);
	cam.setUp(glm::vec3(0, 0, 0));
	scene = std::shared_ptr<tetraRender::Scene>(new tetraRender::Scene(cam));
	scene->load(sceneFile);
}


WindowBuilder::~WindowBuilder()
{
}

void WindowBuilder::draw()
{
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	tetraRender::EventHandler handler(scene);

	sf::Clock deltaClock;
	while (window.isOpen()) {
		window.clear();

		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			handler.handle(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		scene->renderScene();
		ImGui::SFML::Update(window, deltaClock.restart());


		ImGui::Begin("Hello, world!");
		ImGui::Button("Look at this pretty button3333");
		ImGui::End();

		//window.pushGLStates();
		//ImGui::SFML::Render(window);
		window.draw(shape);

		//window.popGLStates();
		window.display();
	}

	ImGui::SFML::Shutdown();


	
}

#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>

#include <iostream>
#include <iomanip>



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
	settings.minorVersion = 5;
	settings.attributeFlags = sf::ContextSettings::Core;

	unsigned width = WIDTH;
	unsigned height = HEIGHT;

	
	window.create(sf::VideoMode(width, height), "Tetra Engine", sf::Style::Close, settings);
	ImGui::SFML::Init(window);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glewExperimental = GL_TRUE;
	glewInit();
	//apparently an old implementation bug tends to raise an error on startup. We call geterror to remove it.
	glGetError();

	//window.resetGLStates();
	
	//std::cout << glGetString(GL_VERSION) << std::endl;
	


}

WindowBuilder::WindowBuilder(std::string sceneFile) : WindowBuilder()
{
	tetraRender::Camera cam(600.0f, 800.0f, 0.75f);
	cam.setUp(glm::vec3(0, 0, 0));
	//new tetraRender::Scene(cam);

	
	//scene = std::shared_ptr<tetraRender::Scene>(new tetraRender::Scene(cam));
	//scene->load(sceneFile);
}


WindowBuilder::~WindowBuilder()
{
}

void WindowBuilder::draw()
{
	//tetraRender::EventHandler handler(scene);
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop


		sf::Event event;
		int time = clock.getElapsedTime().asMilliseconds();
		int frameTime = sf::milliseconds(16).asMilliseconds();
		bool needNewFrame = time >= frameTime;
		//It would seem that without this sync, there is a fall in performance. Not sure why yet. Also the application takes way more resources without it;
		//std::cout << '\r' << std::setw(4) << std::setfill(' ');
		if (needNewFrame)
		{
			//Render time.
			clock.restart();
			//scene->renderScene();
		}
		if (window.pollEvent(event))
		{

			//handler.handle(event);
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();

			}
		}
		ImGui::SFML::ProcessEvent(event);
		ImGui::SFML::Update(window, deltaClock.restart());


		if (ImGui::Button("Update window title")) {
			// this code gets if user clicks on the button
			// yes, you could have written if(ImGui::InputText(...))
			// but I do this to show how buttons work :)
		}
		ImGui::End(); // end window

		ImGui::SFML::Render(window);
		window.display();



	}
}

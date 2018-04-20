#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <iomanip>

#include <crtdbg.h>

#include <tetraRender\Camera.h>
#include <tetraRender\Scene.h>
#include <tetraRender\EventHandler.h>


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

	unsigned width = 1024;
	unsigned height = 720;

	
	window.create(sf::VideoMode(width, height), "Tetra Engine", sf::Style::Close, settings);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glewExperimental = GL_TRUE;
	glewInit();
	
	_ASSERT(_CrtCheckMemory());
	//apparently an old implementation bug tends to raise an error on startup. We call geterror to remove it.
	glGetError();
	_ASSERT(_CrtCheckMemory());
	tetraRender::Camera cam(600.0f, 800.0f, 0.75f);
	cam.setUp(glm::vec3(0, 0, 0));
	_ASSERT(_CrtCheckMemory());
	//new tetraRender::Scene(cam);

	sf::Clock clock;
	_ASSERT(_CrtCheckMemory());
	std::shared_ptr<tetraRender::Scene>  scene(new tetraRender::Scene(cam));
	scene->load("obj/scene.json");
	
	tetraRender:: EventHandler handler(scene);
	//std::cout << glGetString(GL_VERSION) << std::endl;
	

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

			std::cout << '\r' << std::setw(4) << std::setfill(' ') << time;
			clock.restart();
			
			scene->renderScene();
			window.display();
		}
		if (window.pollEvent(event))
		{
			//scene->eventHandler(event);
			handler.handle(event);
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}


	}
}


WindowBuilder::~WindowBuilder()
{
}

void WindowBuilder::draw()
{
}

#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\vec3.hpp>
#include <iostream>

#include "Scene.h"
#include "Polygon.h"
#include "waveFrontLoader.h"
#include "Solid.h"
#include "Cube.h"

//This will build a window using open GL and stuff, this is a way to unclutter the main.
WindowBuilder::WindowBuilder()
{
	const std::string title = "openGL";
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2; // Optional
									// Request OpenGL version 3.2 (optional but recommended)
	settings.majorVersion = 3;
	settings.minorVersion = 2;
	settings.attributeFlags = sf::ContextSettings::Core;

	float width = 800;
	float height = 600;

	window.create(sf::VideoMode(width, height), "openGL", sf::Style::Close, settings);
	glewExperimental = GL_TRUE;
	int success = glewInit();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//if(!success)
	//{
	//	std::cout << "Failure:::";
	//}
	
	WaveFrontLoader loader;
	std::vector<Solid> elem(loader.GetSolidsFromFile("obj/scene.obj"));
	Scene scene(elem);

	glm::mat4 projection = glm::perspective(0.75f, width/height, 0.1f, 200.0f);
	sf::Clock clock;
	float rotation = 1.0f;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		sf::Event event;
		Camera cam(600.0f, 800.0f, 0.75f);
		

		if (clock.getElapsedTime().asMilliseconds() >= sf::milliseconds(30).asMilliseconds())
		{
			glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
			rotation++;
			glm::mat4 rot = glm::rotate(rotation * 0.02f, glm::vec3(0, 0, 1));
			glm::vec3 pos(10.0f, 10.0f, 10.0f);
			cam.setPosition( rot * glm::vec4(pos,1));
			cam.setTarget(glm::vec3(0, 0, 0));
			cam.setUp(glm::vec3(0, 0, 1));
			scene.setCamera(cam);
			clock.restart();
			

		}
		glClear(GL_COLOR_BUFFER_BIT);


		
		scene.renderScene();
		window.display();
		while (window.pollEvent(event))
		{
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

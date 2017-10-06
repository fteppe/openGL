#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\vec3.hpp>
#include <iostream>
#include <iomanip>

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
	glEnable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	glewInit();

	
	WaveFrontLoader loader;
	std::vector<Solid> elem(loader.GetSolidsFromFile("obj/scene.obj"));
	Texture tex;
	tex.loadTexture("textures/texture2.jpg");
	Shader shade("texture.ver", "debug.frag");
	//shade.setDiffuse(tex);
	//elem[2].setShader(shade);
	//elem[4].setShader(shade);
	elem[0].setShader(shade);
	glm::mat4 projection = glm::perspective(0.75f, width/height, 0.1f, 200.0f);
	sf::Clock clock;
	float rotation = 1.0f;
	Camera cam(600.0f, 800.0f, 0.75f);
	//cam.setPosition(glm::vec3(-10, 10, 10));
	Scene scene(elem, cam);
	std::cout << glGetString(GL_VERSION) << std::endl;
	int maxTexSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	std::cout << "size max" << maxTexSize << std::endl;

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sf::Event event;
		
		

		if (clock.getElapsedTime().asMilliseconds()  >= sf::milliseconds(3).asMilliseconds())
		{
			scene.animate(clock);
			//Render time.
			std::cout <<'\r'<< std::setw(3) << std::setfill(' ')<<clock.getElapsedTime().asMilliseconds();
			clock.restart();
			
			scene.renderScene();
			window.display();
		}

		
		
		while (window.pollEvent(event))
		{
			scene.eventHandler(event);
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

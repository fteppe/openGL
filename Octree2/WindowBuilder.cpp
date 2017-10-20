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
#include "ShaderBasic.h"
#include "ShaderSpecular.h"
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

	float width = 1024;
	float height = 720;

	window.create(sf::VideoMode(width, height), "openGL", sf::Style::Close, settings);
	glEnable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	glewInit();

	
	WaveFrontLoader loader;
	std::vector<Solid> elem(loader.GetSolidsFromFile("obj/scene.obj"));
	Texture tex;
	Texture stone;
	stone.loadTexture("textures/texture2.jpg");
	tex.loadTexture("textures/No-Mans-Sky-1.jpg");
	
	std::shared_ptr<Shader> text(new ShaderBasic);
	std::shared_ptr<ShaderSpecular> spec(new ShaderSpecular);
	spec->setDiffuse(stone);
	text->setDiffuse(tex);
	elem[3].setShader(text);
	//elem[0].setShader(shade);
	elem[2].setShader(spec);
	//elem[1].setShader(shade);
	sf::Clock clock;

	Camera cam(600.0f, 800.0f, 0.75f);
	Scene scene(elem, cam);
	//std::cout << glGetString(GL_VERSION) << std::endl;

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sf::Event event;
		int time = clock.getElapsedTime().asMilliseconds();
		int frameTime = sf::milliseconds(1).asMilliseconds();
		bool needNewFrame = time >= frameTime;
		//It would seem that without this sync, there is a fall in performance. Not sure why yet. Also the application takes way more resources;
		std::cout << '\r' << std::setw(4) << std::setfill(' ');
		if (needNewFrame)
		{
			scene.animate(clock);
			//Render time.
			//std::cout << time << std::endl;
			std::cout << '\r' << std::setw(4) << std::setfill(' ') << time;
			clock.restart();
			
			scene.renderScene();
			int error = glGetError();
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

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
	
	
	std::shared_ptr<Texture> stone(new Texture);
	stone->loadTexture("textures/specular_maps_bricks_image.jpg");
	std::shared_ptr<Texture> bump(new Texture);
	bump->loadTexture("textures/4483-normal.jpg");
	std::shared_ptr<Texture> nms(new Texture);
	nms->loadTexture("textures/No-Mans-Sky-1.jpg");
	
	std::shared_ptr<Shader> text(new ShaderBasic);
	std::shared_ptr<ShaderSpecular> spec(new ShaderSpecular);
	std::shared_ptr<Material> mat(new Material(spec.get()));
	std::shared_ptr<Material> mat2(new Material(spec.get()));
	mat->setChannel(stone.get(), "spec");
	mat->setChannel(bump.get(), "bump");
	mat->setChannel(nms.get(), "diffuse");
	mat2->setChannel(stone.get(), "spec");
	mat2->setChannel(bump.get(), "bump");
	mat2->setChannel(nms.get(), "diffuse");
	//spec->setChannel(stone, "spec");
	//spec->setChannel(bump, "bump");
	elem[3].setMaterial(mat2);
	elem[0].setMaterial(mat);
	elem[2].setMaterial(mat2);
	elem[1].setMaterial(mat);
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
		//It would seem that without this sync, there is a fall in performance. Not sure why yet. Also the application takes way more resources without it;
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

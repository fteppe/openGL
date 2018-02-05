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
#include "shaderPBR.h"
#include "waveFrontLoader.h"
#include "Solid.h"
#include "Cube.h"
#include "EventHandler.h"

//This will build a window using open GL and stuff, this is a way to unclutter the main.
WindowBuilder::WindowBuilder()
{
	/*
	WINDOW OPENGL INIT
	*/
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

	std::vector<VertexBufferObject*> vec;
	loader.fillVertexObjectVectorFromFile("obj/scene.obj", vec);
	//We want to control the destruction of our items and follow them with weak ptr
	std::vector<std::shared_ptr<VertexBufferObject>> vec_shared;
	for (int i = 0; i < vec.size(); i++)
	{
		vec_shared.push_back(std::shared_ptr<VertexBufferObject>(vec[i]));
	}


	std::vector<Solid> elem;

	
	std::shared_ptr<Texture> stone(new Texture);
	stone->loadTexture("textures/parallax_mapping_height_map.png");
	std::shared_ptr<Texture> bump(new Texture);
	bump->loadTexture("textures/bricks_normal.jpg");
	std::shared_ptr<Texture> nms(new Texture);
	nms->loadTexture("textures/No-Mans-Sky-1.jpg");
	
	//std::shared_ptr<Shader> text(new ShaderBasic);
	std::shared_ptr<ShaderPBR> spec(new ShaderPBR({ "texture.ver" }, { "PBR.frag","specularCalc.frag","lightCalc.frag" }));
	std::shared_ptr<Material> mat(new Material(spec.get()));
	std::shared_ptr<Material> mat2(new Material(spec.get()));
	mat->setChannel(stone.get(), "spec");
	mat->setChannel(bump.get(), "bump");
	mat->setChannel(nms.get(), "diffuse");
	mat2->setChannel(stone.get(), "spec");
	mat2->setChannel(bump.get(), "bump");
	mat2->setChannel(nms.get(), "diffuse");

	//elem[0].setMaterial(mat);


	for (auto vbo : vec_shared)
	{
		elem.push_back(Solid(vbo));
		elem.back().setMaterial(mat);
	}
	sf::Clock clock;

	Camera cam(600.0f, 800.0f, 0.75f);
	auto  scene = std::shared_ptr<Scene>(new Scene(elem, cam));
	EventHandler handler(scene);
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
			if (window.pollEvent(event))
			{
				//scene->eventHandler(event);
				handler.handle(event);
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//scene.animate(clock);
			//Render time.
			//std::cout << time << std::endl;
			std::cout << '\r' << std::setw(4) << std::setfill(' ') << time;
			clock.restart();
			
			scene->renderScene();
			//int error = glGetError();
			window.display();
		}


	}
}


WindowBuilder::~WindowBuilder()
{
}

void WindowBuilder::draw()
{
}

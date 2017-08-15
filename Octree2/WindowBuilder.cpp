#include "stdafx.h"
#include "WindowBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <glew/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\vec3.hpp>
#include <iostream>

#include "Polygon.h"

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

	window.create(sf::VideoMode(800, 600), "openGL", sf::Style::Close, settings);
	glewInit();
	glewExperimental = GL_TRUE;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//gluPerspective(45.0, float(800) / float(600), 0.1, 100.0);
	
	
	
	Cube cube(0.5f);
	Cube cube2(0.2f);
	sf::Clock clock;
	float rotation = 1.0f;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		sf::Event event;
		if (clock.getElapsedTime().asMilliseconds() >= sf::milliseconds(30).asMilliseconds())
		{
			glm::mat4 transfo = glm::rotate(1.0f,glm::vec3(1.0,0.0,0))*glm::rotate(rotation*0.05f, glm::vec3(0.0, 0.0, 1.0));
			rotation++;
			cube.setObjectSpace(transfo);
			cube2.setObjectSpace( transfo * glm::translate(glm::vec3(0.5, 0.5, 0.0)) * transfo );
			clock.restart();

		}
		glClear(GL_COLOR_BUFFER_BIT);

		cube.draw();
		cube2.draw();
		draw();
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
	
	
	//glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0.5,0.0,0.0));
	//Shader shader("transform.ver", "col.frag");
	std::vector<glm::vec3> points = { -Vec3::u().multiply(0.5),-Vec3::v(),Vec3::u(), Vec3::v() };
	//Solid carre({ points });
	//carre.draw();


}

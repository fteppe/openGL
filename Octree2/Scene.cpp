#include "stdafx.h"
#include "Scene.h"
#include <glm\gtx\transform.hpp>
#include "waveFrontLoader.h"
#include <glm\matrix.hpp>
#include "CubeMap.h"

#include <memory>

Scene::Scene(Camera cam):cam(cam)
{
	light.intensity = 1.0f;
	light.col = glm::vec3(1, 1, 1);
	light.setPos(glm::vec3(1, 1, 0.5));
	this->cam.setPosition(glm::vec3(-5, 0, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 1, 0));
}

/*Scene::Scene(std::vector<Solid> elem, Camera cam) : elements(elem), cam(cam)
{
	light.intensity = 1.0f;
	light.col = glm::vec3(1, 1, 1);
	light.setPos( glm::vec3(1, 1, 0.5));
	this->cam.setPosition(glm::vec3(-10, -10, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 0, 1));
}*/

void Scene::animate(sf::Clock elapsed)
{
	glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
	float rotation = elapsed.getElapsedTime().asMilliseconds();
	//std::cout << rotation << std::endl;
	glm::mat4 rot = glm::rotate(0.002f, glm::vec3(0, 0, 1));
	glm::vec3 pos = cam.getPos();
	//pos = rot * glm::vec4(pos , 1);
	light.setPos( rot * glm::vec4(light.getPos(), 1));//;glm::vec4(5,0, 3,1);



}


Scene::~Scene()
{
	for (GameObject* obj : elements)
	{
		delete obj;
	}
}

void Scene::eventHandler(sf::Event event)
{
	glm::vec3 campPos = cam.getPos();
	glm::vec3 target = cam.getTarget();
	glm::vec3 up = cam.getUp();
	glm::vec3 dir = target - campPos;
	glm::vec3 dirNorm = glm::normalize(dir);
	float distance = glm::length(dir);
	dir = glm::normalize(dir);
	glm::vec3 perpendicular = glm::cross(dir, up);

	//this way the angle when rotating doesn't depend on the distance between the camera and the target.
	perpendicular = perpendicular * (distance / 50);
	up = up * (distance / 50);
	if (event.type == sf::Event::KeyPressed)
	{
		//mouvements lat
		sf::Keyboard keyboard;
		if (keyboard.isKeyPressed(keyboard.Z))
		{
			target = (target + dirNorm);
			campPos = (campPos + dirNorm);
		}
		if (keyboard.isKeyPressed(keyboard.S))
		{
			target = (target - dirNorm);
			campPos = (campPos +-dirNorm);
		}
		if (keyboard.isKeyPressed(keyboard.D))
		{
			
			target = (target + perpendicular);
			campPos = (campPos + perpendicular);
		}
		if (keyboard.isKeyPressed(keyboard.Q))
		{
			target = target - perpendicular;
			campPos -= perpendicular;

		}
		if (keyboard.isKeyPressed(keyboard.Space))
		{
			target += up;
			campPos += up;
		}
		if (keyboard.isKeyPressed(keyboard.LControl))
		{
			target -= up;
			campPos -= up;
		}
		//mouv rot
		if (keyboard.isKeyPressed(keyboard.Left))
		{
			target = target - perpendicular;
		}
		if (keyboard.isKeyPressed(keyboard.Right))
		{
			target = target + perpendicular;
		}
		if (keyboard.isKeyPressed(keyboard.Down))
		{
			target = target - up;
		}
		if (keyboard.isKeyPressed(keyboard.Up))
		{
			target = target + up;
		}
	}
	cam.setTarget(target);
	cam.setPosition(campPos);
}

void Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void Scene::renderScene()
{
	glm::mat4 rot(glm::rotate(0.5f, glm::vec3(1.0, 0, 0)));

	for (int i = 0; i < elements.size(); i++)
	{
		//elements[i].setObjectSpace(rot);
		elements[i]->draw(*this);
	}
}

void Scene::load(std::string scene)
{
	loader.setSceneToLoad(scene);
	models = loader.loadModels();
	textures = loader.loadTextures();
	shaders = loader.loadShaders();
	materials = loader.loadMaterials(textures, shaders);
	elements = loader.loadGameObjects(materials, models);
	makeSkyBox();
}

Camera Scene::getCam() const
{
	return cam;
}

Light Scene::getLight() const
{
	return light;
}

void Scene::makeSkyBox()
{
	//TODO: change this function because this is right now the dirtiest way to do it.
	//to make a skybox we need a cube to draw the texture on
	WaveFrontLoader objLoader;
	std::vector<VertexBufferObject * > vec;
	objLoader.loadVertexObjectVectorFromFile("obj/common.obj", vec);
	for (VertexBufferObject * obj : vec)
	{
		
		if (obj->getFilePath().second == "Cube")
		{
			this->models["obj/common.obj"]["Cube"] = std::shared_ptr<VertexBufferObject>(obj);
		}
	}

	textures["skybox"] = std::shared_ptr<Texture>(new CubeMap);
	std::string textureDir = "textures/skybox/";
	std::vector<std::string> tex = { textureDir + "right.jpg", textureDir + "left.jpg", textureDir + "top.jpg", textureDir + "bottom.jpg",  textureDir + "front.jpg",textureDir + "back.jpg" };
	textures["skybox"]->loadTextures(tex);
	Shader* shaderSky = new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" });
	//shaders["skybox"] = std::shared_ptr<Shader>(new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" }));
	shaders["skybox"] = std::shared_ptr<Shader>(shaderSky);
	materials["sky"] = std::shared_ptr<Material>(new Material(shaders["skybox"].get()));
	materials["sky"]->setChannel(textures["skybox"].get(), "skybox");
	Solid* sky = new Solid(models["obj/common.obj"]["Cube"]);
	sky->setMaterial(materials["sky"]);
	sky->setScale(glm::vec3(100, 100, 100));
	elements.push_back(sky);
}

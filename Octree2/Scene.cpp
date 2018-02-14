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
	this->cam.setPosition(glm::vec3(-5, 1, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 1, 0));

	clock.restart();
	frame = new FrameBuffer;

	frame->renderToScreen();

}


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
	delete skybox;
	delete frame;
}

void Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void Scene::renderScene()
{
	int error = glGetError();
	renderPass = 0;
	frame->renderToThis();
	/*
	for (int i = 0; i < elements.size(); i++)
	{

		elements[i]->setPos(glm::vec3(0));
		elements[i]->draw(*this);
	}
	*/
	elements[1]->draw(*this);
	//textures["textures/EyCkvNyNormal.png"]->readData();
	//textures["reflection"]->readData();
	error = glGetError();
	renderPass = 1;
	frame->renderToScreen();
	elements[1]->draw(*this);
	elements[0]->draw(*this);
	
	error = glGetError();
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


	textures["reflection"] = std::shared_ptr<Texture>(new Texture);
	materials["mat1"]->setChannel(textures["reflection"].get(), "reflectionTex");
	frame->attachOutputTexture(textures["reflection"]);

}

Camera Scene::getCam() const
{
	return cam;
}

Light Scene::getLight() const
{
	return light;
}

std::shared_ptr<Texture> Scene::getTexture(std::string tex) 
{
	std::shared_ptr<Texture> texture =  textures[tex];
	return texture;
}

float Scene::getElapsedTime() const
{
	sf::Time time= clock.getElapsedTime();
	float elapsedTime = time.asSeconds();
	return elapsedTime;
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

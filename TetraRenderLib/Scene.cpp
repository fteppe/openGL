#include "stdafx.h"
#include "Scene.h"
#include <glm/gtx/transform.hpp>
#include "waveFrontLoader.h"
#include <glm/matrix.hpp>
#include "CubeMap.h"
#include "ShaderPostProcess.h"

#include <memory>
#include <crtdbg.h>

using namespace tetraRender;

class CubeMap;
tetraRender::Scene::Scene(Camera cam)
{
	_ASSERT(_CrtCheckMemory());
	setCamera(cam);
	
	renderPipeLine = std::unique_ptr<RenderPipeline>(new RenderPipeline(*this));
	this->cam.setPos(glm::vec3(5, 1, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 1, 0));

	//The shadows of this scene
	this->shadowProjection = this->cam;
	shadowProjection.setPos(glm::vec3(-2, 1, 2));
	shadowProjection.setProjectionOrtho(2, 2, 1, 10);
	clock.restart();
}


void tetraRender::Scene::animate(sf::Clock elapsed)
{
	glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
	float rotation = elapsed.getElapsedTime().asMilliseconds();
	//std::cout << rotation << std::endl;
	glm::mat4 rot = glm::rotate(0.002f, glm::vec3(0, 0, 1));
	glm::vec3 pos = cam.getPos();
	//pos = rot * glm::vec4(pos , 1);
	light.setPos( rot * glm::vec4(light.getPos(), 1));//;glm::vec4(5,0, 3,1);
}


 tetraRender::Scene::~Scene()
{
	for (GameObject* obj : gameObjects)
	{
		delete obj;
	}

}

void tetraRender::Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void tetraRender::Scene::renderScene()
{
	renderPipeLine->renderScene(*this);
}

void tetraRender::Scene::load(std::string scene)
{

	SceneLoader loader;
	loader.setSceneToLoad(scene);

	auto shadersLoaded = loader.loadShaders();
	shaders.insert(shadersLoaded.begin(), shadersLoaded.end());

	auto materialsLoaded = loader.loadMaterials(textures, shaders);
	materials.insert(materialsLoaded.begin(), materialsLoaded.end());

	auto elementsLoaded = loader.loadGameObjects(materials, models);
	gameObjects.insert(gameObjects.end(), elementsLoaded.begin(), elementsLoaded.end());

	makeSkyBox();

	//Testing puposes.

	//setupPostProcessing();


	//Light* light = new Light();
	//light->intensity = 1.0f;
	//light->col = glm::vec3(1, 1, 1);
	//light->setPos(glm::vec3(1, 1, 0.5));
	//light->setParent(gameObjects[0]);
	//gameObjects[0]->addChild(light);

}

Camera tetraRender::Scene::getCam() const
{
	return cam;
}

void tetraRender::Scene::addGameObject(GameObject * obj)
{
	gameObjects.push_back(obj);
}

std::vector<Light*> tetraRender::Scene::getLights(GameObject * root) const
{
	std::vector<Light*> lights;
	GameObjectType type = root->getType();
	if (type == GameObjectType::LIGHT)
	{
		lights.push_back((Light*)root);
	}
	for (auto child : root->getChildren())
	{
		std::vector<Light*> childLights = getLights(child);
		lights.insert(lights.end(), childLights.begin(), childLights.end());
	}
	return lights;
}

Camera tetraRender::Scene::getShadowProj() const
{
	return shadowProjection;
}

Light tetraRender::Scene::getLight() const
{
	return *getLights(gameObjects[0])[0];
}

std::shared_ptr<Texture> tetraRender::Scene::getTexture(std::string tex) 
{
	std::shared_ptr<Texture> texture =  textures[tex];
	return texture;
}

float tetraRender::Scene::getElapsedTime() const
{
	sf::Time time= clock.getElapsedTime();
	float elapsedTime = time.asSeconds();
	return elapsedTime;
}

std::vector<GameObject*> tetraRender::Scene::getGameObjects()
{
	return gameObjects;
}

std::vector<Light*> tetraRender::Scene::getLights()
{
	std::vector<Light*> lights;
	//For each game objects at the root, we look for the lights in their children.
	for (auto go : gameObjects)
	{
		std::vector<Light* > get = getLights(go);
		lights.insert(lights.end(), get.begin(), get.end());
	}
	return lights;
}

void tetraRender::Scene::makeSkyBox()
{
	//TODO: change this function because this is right now the dirtiest way to do it.
	//to make a skybox we need a cube to draw the texture on
	WaveFrontLoader objLoader;
	std::vector<Mesh * > vec;
	objLoader.loadVertexObjectVectorFromFile("obj/common.obj", vec);
	bool found = false;
	
	for (Mesh * obj : vec)
	{
		
		if (obj->getFilePath().second == "Cube")
		{
			found = true;
			this->models["obj/common.obj"]["Cube"] = std::shared_ptr<Mesh>(obj);
		}
		else
		{
			//if this isn't the cube we delete it.
			delete obj;
		}
	}
	if(found == false)
	{
		std::cout<<__FILE__<<"::"<<__LINE__<<"ERROR: element not found"<<std::endl;
	}

	textures["skybox"] = std::shared_ptr<Texture>(new CubeMap);
	std::string textureDir = "textures/skybox/";
	std::vector<std::string> tex = { textureDir + "right.jpg", textureDir + "left.jpg", textureDir + "top.jpg", textureDir + "bottom.jpg",  textureDir + "front.jpg",textureDir + "back.jpg" };
	textures["skybox"]->loadTextures(tex);
	Shader* shaderSky = new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" });
	//shaders["skybox"] = std::shared_ptr<Shader>(new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" }));
	shaders["skybox"] = std::shared_ptr<Shader>(shaderSky);
	materials["sky"] = std::shared_ptr<Material>(new Material(shaders["skybox"]));
	materials["sky"]->setChannel(textures["skybox"], "skybox");
	Solid* sky = new Solid(models["obj/common.obj"]["Cube"]);
	sky->setMaterial(materials["sky"]);
	sky->setScale(glm::vec3(100, 100, 100));
	//sky->addTag(WORLD_OBJECT);
	sky->addTag(FORWARD_RENDER);
	gameObjects.push_back(sky);
}

void tetraRender::Scene::updateShadowMaps()
{
	//We get all the lights.
	std::vector<Light*> lights = getLights();;
	int i = 0;

	//We get the camera's and update the map. 
	for (auto light : lights)
	{
		//This needs to be done better by adding cameras to light but a good chunk is done already.
		if (light->getHasShadow() == true)
		{
			Camera* lightProj = light->getShadowProjection();
			renderPipeLine->setShadowPoV(&cam, i);
		}
		
	}
}


#include "stdafx.h"
#include "Scene.h"
#include <glm/gtx/transform.hpp>
#include "waveFrontLoader.h"
#include <glm/matrix.hpp>
#include "CubeMap.h"
#include "ShaderPostProcess.h"

#include<iostream>
#include <memory>
#include <crtdbg.h>

using namespace tetraRender;

class CubeMap;
tetraRender::Scene::Scene(Camera cam)
{
	_ASSERT(_CrtCheckMemory());
	setCamera(cam);
	gameObjects = new GameObject();
	gameObjects->setName("sceneRoot");
	renderPipeLine = std::unique_ptr<RenderPipeline>(new RenderPipeline(*this));
	this->cam.setPos(glm::vec3(5, 1, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 1, 0));

	//The shadows of this scene
}


void tetraRender::Scene::animate(float elapsed)
{
	glm::mat4 camPos = glm::lookAt(glm::vec3(0.0, 5.0, 2.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	
	float rotation = elapsed;
	//std::cout << rotation << std::endl;
	glm::mat4 rot = glm::rotate(0.002f, glm::vec3(0, 0, 1));
	glm::vec3 pos = cam.getPos();
	//pos = rot * glm::vec4(pos , 1);
}


 tetraRender::Scene::~Scene()
{

	delete gameObjects;


}

void tetraRender::Scene::setCamera(Camera camera) 
{
	cam =  (camera);
}

void tetraRender::Scene::renderScene()
{
	//We make sure the right shadows are being rendered, we send to the pipeline which PoV must be rendered.
	updateShadowMaps();
	//We then start the whole rendering pipeline that is handled by a specific object
	//this pipeline includes shadowmaps rendering and the rendering of the scene.
	renderPipeLine->renderScene(*this);
}

void tetraRender::Scene::load(std::string scene)
{

	SceneLoader loader;
	loader.setSceneToLoad(scene);
	loader.loadTextures(resources);
	//textures.insert(texturesLoaded.begin(), texturesLoaded.end());

	loader.loadShaders(resources);
	//shaders.insert(shadersLoaded.begin(), shadersLoaded.end());

	loader.loadMaterials(resources);
	//materials.insert(materialsLoaded.begin(), materialsLoaded.end());

	auto elementsLoaded = loader.loadGameObjects(resources);
	for (auto go : elementsLoaded)
	{
		gameObjects->addChild(go);
	}

	makeSkyBox();
	renderPipeLine->update(*this);
}

Camera tetraRender::Scene::getCam() const
{
	return cam;
}

void tetraRender::Scene::addGameObject(GameObject * obj)
{
	gameObjects->addChild(obj);
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




std::shared_ptr<Texture> tetraRender::Scene::getTexture(std::string tex) 
{
	return resources.getTexture(tex);
}

float tetraRender::Scene::getElapsedTime() const
{
	float elapsedTime = 0.0;
	return elapsedTime;
}

GameObject* tetraRender::Scene::getGameObjects()
{
	return gameObjects;
}

std::vector<Light*> tetraRender::Scene::getLights()
{
	std::vector<Light*> lights;
	//For each game objects at the root, we look for the lights in their children.

		std::vector<Light* > get = getLights(gameObjects);
		lights.insert(lights.end(), get.begin(), get.end());
	
	return lights;
}

ResourceAtlas & tetraRender::Scene::getResources()
{
	return resources;
}

RenderPipeline & tetraRender::Scene::getRenderPipeLine()
{
	return *this->renderPipeLine;
}

void tetraRender::Scene::makeSkyBox()
{
	//TODO: change this function because this is right now the dirtiest way to do it.
	//to make a skybox we need a cube to draw the texture on
	WaveFrontLoader objLoader;
	std::vector<Mesh * > vec;
	objLoader.loadVertexObjectVectorFromFile("obj/common.obj", vec);
	bool found = false;
	std::shared_ptr<Mesh> mesh = nullptr;
	for (Mesh * obj : vec)
	{
		
		if (obj->getFilePath().second == "Cube")
		{
			found = true;
			mesh = std::shared_ptr<Mesh>(obj);
			resources.addMesh(mesh);
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

	//textures["skybox"] = std::shared_ptr<Texture>(new CubeMap);
	std::string textureDir = "textures/skybox/";
	std::vector<std::string> tex = { textureDir + "right.jpg", textureDir + "left.jpg", textureDir + "top.jpg", textureDir + "bottom.jpg",  textureDir + "front.jpg",textureDir + "back.jpg" };
	//textures["skybox"]->loadTextures(tex);
	Shader* shaderSky = new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" });
	//shaders["skybox"] = std::shared_ptr<Shader>(new Shader(std::vector<std::string>({ "cubeMap.ver" }), { "cubeMap.frag" }));
	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(shaderSky);
	shader->setName("skybox");
	resources.addShader(shader);
	std::shared_ptr<Material> mat = std::shared_ptr<Material>(new Material(shader));
	mat->setChannel(resources.getTexture("skybox"), "skybox");
	mat->setName("skybox");
	Solid* sky = new Solid(mesh);
	resources.addMaterial(mat);
	resources.addShader(shader);
	sky->setMaterial(mat);
	sky->setScale(glm::vec3(100, 100, 100));
	//sky->addTag(WORLD_OBJECT);
	sky->addTag(FORWARD_RENDER);
	addGameObject(sky);
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
			renderPipeLine->setShadowPoV(lightProj, i);
			i++;
		}
		
	}
	renderPipeLine->setNumShadows(i);
}


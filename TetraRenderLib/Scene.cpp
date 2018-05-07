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
	
	
	this->cam.setPosition(glm::vec3(5, 1, 5));
	this->cam.setTarget(glm::vec3(0, 0, 0));
	this->cam.setUp(glm::vec3(0, 1, 0));

	//The shadows of this scene
	this->shadowProjection = this->cam;
	shadowProjection.setPosition(glm::vec3(-2, 1, 2));
	shadowProjection.setProjectionOrtho(2, 2, 1, 10);

	std::shared_ptr<Texture> normalsBuffer(new Texture);
	std::shared_ptr<Texture> depthBuffer(new Texture);
	std::shared_ptr<Texture> spec(new Texture);
	std::shared_ptr<Texture> fragPos(new Texture);
	textures["color"] = std::shared_ptr<Texture>(new Texture);
	textures["normals"] = normalsBuffer;
	textures["depth"] = depthBuffer;
	textures["specularity"] = spec;
	textures["fragPos"] = fragPos;
	textures["shadowDistance"] = std::shared_ptr<Texture>(new Texture);

	clock.restart();
	FrameBuffer * frame = new FrameBuffer;
	frame->setHDR(true);
	//frame->renderToScreen();

	//ShadowMap frameBuffer
	textures["shadowMap"] = std::shared_ptr<Texture>(new Texture);
	frame->setDepthTexture(textures["shadowMap"]);
	//shadow Passes
	renderPasses.push_back(new RenderPass());
	RenderPass * pass = renderPasses.back();
	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ WORLD_OBJECT });
	pass->setCamera(&this->shadowProjection);
	//We create a specific material that is very simple to render everything through it.
	shaders["transform"] = std::shared_ptr<Shader>( new Shader("transform.ver", "transform.frag"));
	materials["shadowMapMat"] = std::shared_ptr<Material>(new Material(shaders["transform"]));
	pass->setMat(materials["shadowMapMat"]);

	
	//Colors renderPass.
	frame = new FrameBuffer;
	frame->addColorOutputTexture(textures["color"]);
	frame->setDepthTexture(depthBuffer);
	frame->addColorOutputTexture(normalsBuffer);
	frame->addColorOutputTexture(spec);
	frame->addColorOutputTexture(fragPos);
	frame->addColorOutputTexture(textures["shadowDistance"]);

	//We set the frame as the renderPass we want for the colors.
	renderPasses.push_back(new RenderPass());
	pass = renderPasses.back();
	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ WORLD_OBJECT });
	pass->setCamera(&this->cam);

	
	//the second render pass we don't set a frameBuffer so it will render to the screen.
	renderPasses.push_back(new RenderPass());
	pass = renderPasses.back();
	pass->setRenderTagsIncluded({ POST_PROCESS });
	pass->setCamera(&this->cam);
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
	for (RenderPass* pass : renderPasses)
	{
		delete pass;
	}
}

void tetraRender::Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void tetraRender::Scene::renderScene()
{
	//It is very simple for now, but I don't believe this is how it should be done. With a hard coded pipeline
	//Or a way to define specific steps I give myself the power to do more stuffs with stencil buffers and other.
	for (auto pass : renderPasses)
	{
 		pass->renderScene(*this);
		//textures["depth"]->readData();
	}
	//renderPasses[0]->renderScene(*this);
	//renderPasses[1]->renderScene(*this);

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

	setupPostProcessing();


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

void tetraRender::Scene::makeSkyBox()
{
	//TODO: change this function because this is right now the dirtiest way to do it.
	//to make a skybox we need a cube to draw the texture on
	WaveFrontLoader objLoader;
	std::vector<VertexBufferObject * > vec;
	objLoader.loadVertexObjectVectorFromFile("obj/common.obj", vec);
	bool found = false;
	
	for (VertexBufferObject * obj : vec)
	{
		
		if (obj->getFilePath().second == "Cube")
		{
			found = true;
			this->models["obj/common.obj"]["Cube"] = std::shared_ptr<VertexBufferObject>(obj);
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
	sky->addTag(WORLD_OBJECT);
	gameObjects.push_back(sky);
}

void tetraRender::Scene::setupPostProcessing()
{
	//All this part is very exeprimental and hard coded weirdly which I would like to change in the future.

	//for the post processing we need a flat triangle
	std::vector<std::vector<int>> faces = { {0,1,2} };
	std::vector<glm::vec3> points = { glm::vec3(-1,-1,0), glm::vec3(3,-1,0),glm::vec3(-1,3,0) };
	VertexBufferObject * screen = new VertexBufferObject(points, faces);
	//the UVs of our screen, the way it is set up we should have the sides of the screen aligned with the side of the square in the triangle.
	std::vector<glm::vec3> uvs = { glm::vec3(0,0,0), glm::vec3(2,0,0), glm::vec3(0,2,0) };
	screen->setUVs(uvs);
	//we give it a name so we can more easely follow it.
	screen->setFilePath(std::pair<std::string, std::string>("hard", "screen"));
	std::shared_ptr<VertexBufferObject> vbo_ptr(screen);
	Solid* screenObj = new Solid(vbo_ptr);
	//models["hard"]["screen"] = vbo_ptr;
	//this triangle has a different shader than usual.

	Shader* shader = new ShaderPostProcess({ "postProcess.ver" },  { "lightCalc.frag", "defferedShading.frag" });
	std::shared_ptr<Shader> shader_ptr(shader);
	Material* mat = new Material(shader_ptr);
	std::shared_ptr<Material> postProcessMat(mat);
	postProcessMat->setChannel(textures["color"], "color");
	postProcessMat->setChannel(textures["normals"], "normals");
	postProcessMat->setChannel(textures["depth"], "depth");
	postProcessMat->setChannel(textures["specularity"], "specularity");
	postProcessMat->setChannel(textures["fragPos"], "fragPos");
	postProcessMat->setChannel(textures["shadowDistance"], "shadowDistance");
	postProcessMat->setChannel(textures["shadowMap"], "shadowMap");

	screenObj->setMaterial(postProcessMat);
	screenObj->addTag(POST_PROCESS);
	//We add all these newly created elements to the scene;
	shaders["postProcess"] = shader_ptr;
	materials["postProcess"] = postProcessMat;
	gameObjects.push_back(screenObj);	
}

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
	FrameBuffer * frame = new FrameBuffer;

	frame->renderToScreen();
	
	std::shared_ptr<Texture> output2(new Texture);
	textures["color"] = std::shared_ptr<Texture>(new Texture);
	textures["normals"] = output2;
	frame->addColorOutputTexture(textures["color"]);
	frame->addColorOutputTexture(output2);

	//We set the frame as the renderPass we want.
	renderPasses.push_back(new RenderPass());
	RenderPass * pass = renderPasses.back();
	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ WORLD_OBJECT });
	
	//the second render pass we don't set a frameBuffer so it will render to the screen.
	renderPasses.push_back(new RenderPass());
	pass = renderPasses.back();
	pass->setRenderTagsIncluded({ POST_PROCESS });
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
	for (GameObject* obj : gameObjects)
	{
		delete obj;
	}
	for (RenderPass* pass : renderPasses)
	{
		delete pass;
	}
}

void Scene::setCamera(Camera camera) 
{
	cam = (camera);
}

void Scene::renderScene()
{

	for (auto pass : renderPasses)
	{
		pass->renderScene(*this);
	}

}

void Scene::load(std::string scene)
{
	//We add the loaded elements to the ones already existing in the scene.
	loader.setSceneToLoad(scene);
	auto loadedModels = loader.loadModels();
	models.insert(loadedModels.begin(), loadedModels.end());
	
	auto texturesLoaded = loader.loadTextures();
	textures.insert(texturesLoaded.begin(), texturesLoaded.end());

	auto shadersLoaded = loader.loadShaders();
	shaders.insert(shadersLoaded.begin(), shadersLoaded.end());

	auto materialsLoaded = loader.loadMaterials(textures, shaders);
	materials.insert(materialsLoaded.begin(), materialsLoaded.end());

	auto elementsLoaded = loader.loadGameObjects(materials, models);
	gameObjects.insert(gameObjects.end(), elementsLoaded.begin(), elementsLoaded.end());

	makeSkyBox();

	//Testing puposes.

	setupPostProcessing();


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

std::vector<GameObject*> Scene::getGameObjects()
{
	return gameObjects;
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
	materials["sky"] = std::shared_ptr<Material>(new Material(shaders["skybox"]));
	materials["sky"]->setChannel(textures["skybox"], "skybox");
	Solid* sky = new Solid(models["obj/common.obj"]["Cube"]);
	sky->setMaterial(materials["sky"]);
	sky->setScale(glm::vec3(100, 100, 100));
	sky->addTag(WORLD_OBJECT);
	gameObjects.push_back(sky);
}

void Scene::setupPostProcessing()
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

	Shader* shader = new Shader("postProcess.ver", "postProcess.frag");
	std::shared_ptr<Shader> shader_ptr(shader);
	Material* mat = new Material(shader_ptr);
	std::shared_ptr<Material> mat_ptr(mat);
	mat_ptr->setChannel(textures["color"], "color");
	mat_ptr->setChannel(textures["normals"], "normals");
	screenObj->setMaterial(mat_ptr);
	screenObj->addTag(POST_PROCESS);
	//We add all these newly created elements to the scene;
	shaders["postProcess"] = shader_ptr;
	materials["postProcess"] = mat_ptr;
	gameObjects.push_back(screenObj);	
}

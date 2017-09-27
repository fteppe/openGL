#pragma once
#include "Camera.h"
#include "Light.h"
#include "Solid.h"
#include <vector>

/*
This class contains objects, a light and a camera. So we should be able to render a scene.
*/
class Scene
{
public:
	Scene();
	Scene(std::vector<Solid> elem);
	~Scene();

	void setCamera(Camera &camera);
	void renderScene();
private:
	Camera cam;
	Light light;
	std::vector<Solid> elements;
};


#pragma once
#include <tetraRender\ResourceAtlas.h>
class ResourcesLibrary
{
public:
	ResourcesLibrary();
	ResourcesLibrary(tetraRender::ResourceAtlas* resourcesIn);
	void display();
	~ResourcesLibrary();
private:
	tetraRender::ResourceAtlas* resources;
	int matEditorOpen;
	int textureEditorOpen;
	int shaderEditorOpen;
};


#include "ShaderPostProcess.h"
#include "Camera.h"
#include "Scene.h"
#include "Solid.h"
#include <sstream>

using namespace tetraRender;

ShaderPostProcess::ShaderPostProcess()
{
}

tetraRender::ShaderPostProcess::ShaderPostProcess(std::vector<std::string> vertexShaders, std::vector<std::string> fragmentShaders) : Shader(vertexShaders, fragmentShaders)
{
}


ShaderPostProcess::~ShaderPostProcess()
{
}

void tetraRender::ShaderPostProcess::setProgramInformation(tetraRender::Scene & scene, const Solid & solid)
{
	Shader::setProgramInformation(scene, solid);

	Camera cam = scene.getCam();
	glm::mat4 cameraSpace = cam.getProjection();
	sendMatrix4("inverseCam", glm::inverse(cameraSpace));

	float time = scene.getElapsedTime();
	sendFloat("time", time);

	std::vector<Light *> lights = scene.getLights();
	//We send the light data but also the data concerning shadows. This is a bit tricky since this information is a bit "deep in our data structures".
	//Because of how it is setup we consider that the first light we find with a shadow projection is asigned the first shadowmap etc...
	unsigned indexShadow = 0;
	for (int i = 0; i < lights.size(); i++)
	{
		std::stringstream ss;
		Light* light = lights[i];
		ss << "lights[" << i << "]";
		sendLight(ss.str(), *light);
		if (light->getHasShadow())
		{
			//It doesn't work yet!!!
			//scene.getRenderPipeLine().getShadowMapsPass();
			if (indexShadow < scene.getRenderPipeLine().getShadowMapsPass().size())
			{
				auto& shadowPass = scene.getRenderPipeLine().getShadowMapsPass()[indexShadow];
				Camera* shadowProj = shadowPass->getCamera();

				if (shadowProj != NULL)
				{
					glm::mat4 projMat = shadowProj->getProjection();
					std::shared_ptr<Texture> shadowMap = shadowPass->getFrameBuffer().getDepthTexture();
					std::stringstream nameShadowMap;
					nameShadowMap << "shadowMaps[" << indexShadow << "]";
					sendTexture(nameShadowMap.str(), shadowMap);
					std::stringstream nameShadowProj;
					nameShadowProj << "shadowProjection[" << indexShadow << "]";
					sendMatrix4(nameShadowProj.str(), projMat);
					sendInt(ss.str() + ".shadowIndex", indexShadow);
				}
				
			}
			
		}

	}
	sendInt("numLights", lights.size());

	sendVec3("camPos", scene.getCam().getPos());
	
}

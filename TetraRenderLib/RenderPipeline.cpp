#include "RenderPipeline.h"
#include "ShaderPostProcess.h"
#include <functional>

using namespace tetraRender;

RenderPipeline::RenderPipeline(tetraRender::Scene &scene)
{
	//std::unique_ptr<RenderPass> rp(new RenderPass);
	//We have to move the pointer since it is unique, so we have to transfer it and not do a simple copy.
	//renderPasses.push_back(std::move(rp
	setupRenderPasses();
	setupPostProcessing(scene);
}


RenderPipeline::~RenderPipeline()
{
}

void tetraRender::RenderPipeline::renderScene(tetraRender:: Scene & scene)
{
	//We use references because we iterate over a vector of unique ptr.
	//we use const because we wont modify the pass.
	unsigned int i = 0;
	for (auto const & pass : this->shadowmapsPasses)
	{
		if (i < numShadows)
		{
			pass->getFrameBuffer().clear(GL_DEPTH_BUFFER_BIT);
			pass->renderScene(scene);
		}
		i++;
	}
	/*for (auto const & pass : this->renderPasses)
	{*/

	//We render the first elements without lights to a texture.
	auto  pass = std::ref(renderPasses[0]);
	pass.get()->getFrameBuffer().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pass.get()->renderScene(scene);

	//The second pass is where the shading is done.
	pass = std::ref(renderPasses[1]);
	pass.get()->getFrameBuffer().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pass.get()->renderScene(scene);

	//The next step is done so we can render new elements of the scene AFTER deferred shading.
	//For that we copy the depth buffer to the deferred shading pass.
	glm::vec2 frameSize = pass.get()->getFrameBuffer().getSize();
	renderPasses[0]->getFrameBuffer().bind(GL_READ_FRAMEBUFFER);
	renderPasses[1]->getFrameBuffer().bind(GL_DRAW_FRAMEBUFFER);
	glm::vec2 frameSize2 = renderPasses[1]->getFrameBuffer().getSize();
	glBlitFramebuffer(0, 0, frameSize.x, frameSize.y, 0, 0, frameSize2.x, frameSize2.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	pass.get()->getFrameBuffer().bind();
	//We now render the elements of the scene that are forward rendered.
	renderPasses[2]->renderScene(scene);

	auto & finalPass = renderPasses[3];
	finalPass->getFrameBuffer().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	finalPass->renderScene(scene);

}

	void tetraRender::RenderPipeline::setShadowPoV(Camera* PoV, int index)
	{
		//We make sure the index is part of the number of passes.
		if (index < shadowmapsPasses.size())
		{
			shadowmapsPasses[index]->setCamera(PoV);
		}
	}

	void tetraRender::RenderPipeline::setNumShadows(unsigned int numShadows)
	{
		this->numShadows = numShadows;
	}

	void tetraRender::RenderPipeline::update(Scene & scene)
	{
		auto postProcessMat = renderPasses[1]->getMaterial();
		auto sky = scene.getTexture("skybox");
		postProcessMat->setChannel(sky, "skybox");
	}

	std::vector<std::unique_ptr<RenderPass>>& tetraRender::RenderPipeline::getShadowMapsPass()
	{
		return shadowmapsPasses;
	}


void tetraRender::RenderPipeline::setupRenderPasses()
{
	//We create the material that is used to render the scene from the PoV of a light.
	std::shared_ptr<Shader> shaderShadowPass = std::shared_ptr<Shader>(new Shader("transform.ver", "transform.frag"));
	std::shared_ptr<Material> shadowMapsMat = std::shared_ptr<Material>(new Material(shaderShadowPass));
	//pass->setMat(shadowMapsMat);

	//We take the maximum number of shadowmap and create a renderpass for each.
	//If there are less shadowpas than possible shadows we waste some space but consider it an acceptable tradeof.
	std::unique_ptr<RenderPass> shadowPass;
	std::shared_ptr<FrameBuffer> shadowBuffer;
	for (unsigned i = 0; i < MAX_NUM_SHADOWMAP; i++)
	{
		shadowPass.reset(new RenderPass);
		//All shadowMaps renderPasses share the same material that just renders geometry.
		shadowPass->setMat(shadowMapsMat);
		shadowBuffer.reset( new FrameBuffer);
		//We add a new texture to the frameBuffer to which will be written the shadowMap.
		shadowBuffer->setDepthTexture(std::shared_ptr< Texture>(new Texture));
		//We use std::move because we manipulate unique ptr; After that shadowPass cannot be used unless it is reset or it will crash the app.
		
		shadowPass->setRenderOutput(shadowBuffer);
		shadowPass->setRenderTagsIncluded({ WORLD_OBJECT });
		shadowmapsPasses.push_back(std::move(shadowPass));
	}



	//We create the different textures for the GBuffer.
	std::shared_ptr<Texture> normalsBuffer(new Texture);
	std::shared_ptr<Texture> depthBuffer(new Texture);
	std::shared_ptr<Texture> spec(new Texture);
	std::shared_ptr<Texture> fragPos(new Texture);
	gBuffer["color"] = std::shared_ptr<Texture>(new Texture);
	gBuffer["normals"] = normalsBuffer;
	gBuffer["depth"] = depthBuffer;
	gBuffer["specularity"] = spec;
	gBuffer["fragPos"] = fragPos;
	//gBuffer["shadowDistance"] = std::shared_ptr<Texture>(new Texture);


	std::shared_ptr<FrameBuffer> frame (new FrameBuffer);
	frame->setHDR(true);

	frame->addColorOutputTexture(gBuffer["color"]);
	frame->addColorOutputTexture(normalsBuffer);
	frame->addColorOutputTexture(spec);
	frame->addColorOutputTexture(fragPos);
	//frame->addColorOutputTexture(gBuffer["shadowDistance"]);


	std::unique_ptr<RenderPass> pass = std::unique_ptr<RenderPass>(new RenderPass);
	//We set the frame as the renderPass we want for the colors.

	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ WORLD_OBJECT });
	//pass->setCamera(&this->cam);
	// since pass is a unique_ptr we move it inside the vector.
	renderPasses.push_back(std::move(pass));

	//the second render pass we don't set a frameBuffer so it will render to the screen.
	pass.reset(new RenderPass);

	frame.reset(new FrameBuffer);
	frame->setHDR(true);
	std::shared_ptr<Texture> fullColor(new Texture);
	std::shared_ptr<Texture> bright(new Texture);
	gBuffer["fullColor"] = fullColor;
	gBuffer["bright"] = bright;
	frame->addColorOutputTexture(fullColor);
	frame->addColorOutputTexture(bright);
	frame->setDepthTexture(depthBuffer);
	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ POST_PROCESS });
	renderPasses.push_back(std::move(pass));
	//pass->setCamera(&this->cam);
	

	
	pass.reset(new RenderPass);
	pass->setRenderOutput(frame);
	pass->setRenderTagsIncluded({ FORWARD_RENDER });
	renderPasses.push_back(std::move(pass));

	pass.reset(new RenderPass);
	pass->setRenderTagsIncluded({ POST_PROCESS });
	renderPasses.push_back(std::move(pass));

}

void tetraRender::RenderPipeline::setupPostProcessing(Scene & scene)
{
	//All this part is very exeprimental and hard coded weirdly which I would like to change in the future.

	//for the post processing we need a flat triangle
	std::vector<std::vector<int>> faces = { { 0,1,2 } };
	std::vector<glm::vec3> points = { glm::vec3(-1,-1,0), glm::vec3(3,-1,0),glm::vec3(-1,3,0) };
	Mesh * screen = new Mesh(points, faces);
	//the UVs of our screen, the way it is set up we should have the sides of the screen aligned with the side of the square in the triangle.
	std::vector<glm::vec3> uvs = { glm::vec3(0,0,0), glm::vec3(2,0,0), glm::vec3(0,2,0) };
	screen->setUVs(uvs);
	//we give it a name so we can more easely follow it.
	screen->setFilePath(std::pair<std::string, std::string>("hard", "screen"));
	std::shared_ptr<Mesh> vbo_ptr(screen);
	Solid* screenObj = new Solid(vbo_ptr);
	//models["hard"]["screen"] = vbo_ptr;
	//this triangle has a different shader than usual.

	Shader* shader = new ShaderPostProcess({ "postProcess.ver" }, { "defferedShading.frag" });
	std::shared_ptr<Shader> shader_ptr(shader);
	Material* mat = new Material(shader_ptr);
	std::shared_ptr<Material> postProcessMat(mat);
	postProcessMat->setChannel(gBuffer["color"], "color");
	postProcessMat->setChannel(gBuffer["normals"], "normals");
	postProcessMat->setChannel(gBuffer["depth"], "depth");
	postProcessMat->setChannel(gBuffer["specularity"], "specularity");
	postProcessMat->setChannel(gBuffer["fragPos"], "fragPos");

	//postProcessMat->setChannel(gBuffer["shadowDistance"], "shadowDistance");
	//postProcessMat->setChannel(gBuffer["shadowMap"], "shadowMap");

	renderPasses[1]->setMat(postProcessMat);
	screenObj->setMaterial(postProcessMat);
	screenObj->addTag(POST_PROCESS);
	//We add all these newly created elements to the scene;
	
	//Push back is a bit complicated with a unique ptr;
	
	std::shared_ptr<Shader> shaderEffects(new ShaderPostProcess({ "postProcess.ver" }, { "postProcessEffect.frag" }));
	std::shared_ptr<Material> materialEffects(new Material(shaderEffects));
	materialEffects->setChannel(gBuffer["fullColor"], "fullColor");
	materialEffects->setChannel(gBuffer["bright"], "bright");
	materialEffects->setChannel(gBuffer["depth"], "depth");
	renderPasses.back()->setMat(materialEffects);

	scene.addGameObject(screenObj);
}

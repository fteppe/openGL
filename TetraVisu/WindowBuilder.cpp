#include "stdafx.h"
#include "WindowBuilder.h"
#include <glew/glew.h>

#include <iostream>

#include <tetraRender/resource.h>
#include <tetraRender\Scene.h>
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>


//This will build a window using open GL and stuff, this is a way to unclutter the main.
WindowBuilder::WindowBuilder()
{
	std::cout << "starting application" << std::endl;
	/*
	WINDOW OPENGL INIT
	*/
	const std::string title = "openGL";


	unsigned width = WIDTH;
	unsigned height = HEIGHT;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
	}

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	window = SDL_CreateWindow("ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE| SDL_RENDERER_PRESENTVSYNC);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplSdlGL3_Init(window);

	// Setup style

	
	//apparently an old implementation bug tends to raise an error on startup. We call geterror to remove it.
	glGetError();

	
	
	std::cout << glGetString(GL_VERSION) << std::endl;
	


}

WindowBuilder::WindowBuilder(std::string sceneFile) : WindowBuilder()
{
	tetraRender::Camera cam(600.0f, 800.0f, 0.75f);
	cam.setUp(glm::vec3(0, 0, 0));
	scene = std::shared_ptr<tetraRender::Scene>(new tetraRender::Scene(cam));
	scene->load(sceneFile);
	handler = EventHandler(scene);

}


WindowBuilder::~WindowBuilder()
{
}

void WindowBuilder::draw()
{


	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			handler.handle(event);
			if (event.type == SDL_QUIT)
				done = true;
		}
		scene->renderScene();
		ImGui_ImplSdlGL3_NewFrame(window);

		ImGui::Begin("Material editor");
		for (auto material : scene->getMaterials())
		{
			this->MaterialUI(material.second);
		}
		ImGui::End();
		ImGui::Begin("GameObjects");
		int i = 0;
		for (auto go : scene->getGameObjects())
		{
			gameObjectTreeUI(go,i);
			i++;
		}
		ImGui::End();

		ImGui::Begin("Game object editor");
		if (selectedObject)
		{
			gameObjectEditUI(selectedObject);
		}
		ImGui::End();
		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	
}

glm::vec3 WindowBuilder::Vec3Input(glm::vec3 vector, std::string label)
{
	float vec[3] = { vector.x, vector.y, vector.z };
	//Imgui::InputFloat3(param.first.c_str(), vec, 5);
	ImGui::InputFloat3(label.c_str(), vec, 2);
	vector.x = vec[0]; vector.y = vec[1]; vector.z = vec[2];
	return vector;
}

void WindowBuilder::MaterialUI(std::shared_ptr<tetraRender::Material> mat)
{
	ImGui::Text("--------");
	auto& val = mat->getParameters();
	ImGui::Text(mat->getName().c_str());
	for (auto param : val.getParameters())
	{
		//if the parameter is a vector.
		if (param.second == tetraRender::ParameterType::VEC3)
		{
			glm::vec3 vector = val.getVec3(param.first);
			vector = Vec3Input(vector, param.first.c_str());
			val.set(param.first, vector);

		}
	}
	ImGui::Text("--------");
}

void WindowBuilder::gameObjectTreeUI(tetraRender::GameObject * gameObject, int pos)
{
	std::string name = gameObject->getName();
	auto children = gameObject->getChildren();
	if (children.size() > 0)
	{
		if (ImGui::TreeNode(std::to_string(pos).c_str(), name.c_str()))
		{
			int i = 0;
			for (auto child : gameObject->getChildren())
			{
				gameObjectTreeUI(child,i);
			}
		}
		if (ImGui::IsItemClicked())
		{
			selectedObject = gameObject;
		}

		ImGui::TreePop();
	}
	else
	{
		if (ImGui::Button(name.c_str()), std::to_string(pos).c_str())
		{
		}
		if (ImGui::IsItemClicked())
		{
			selectedObject = gameObject;
		}
	}


	


}

void WindowBuilder::gameObjectEditUI(tetraRender::GameObject * gameObject)
{
	ImGui::Text(selectedObject->getName().c_str());
	auto& paramContainer = selectedObject->getParameters();
	for (auto param : paramContainer.getParameters())
	{
		if (param.second == tetraRender::ParameterType::VEC3)
		{
			glm::vec3 vector = selectedObject->getParameters().getVec3(param.first);
			vector = Vec3Input(vector, param.first.c_str());
			paramContainer.set(param.first, vector);

		}
		else if (param.second == tetraRender::ParameterType::FLOAT)
		{
			float val = selectedObject->getParameters().getFloat(param.first);
			val = ImGui::InputFloat(param.first.c_str(), &val,0.05f);
			paramContainer.set(param.first, val);
		}
	}
	selectedObject->update();
}

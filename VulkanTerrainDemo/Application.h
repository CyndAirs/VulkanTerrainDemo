#pragma once

#include "Shared.h"
#include "Renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application
{
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
	}

	Application();

	~Application();

private:

	GLFWwindow* window;

	//Renderer renderer;

	void initWindow();

	void closeWindow();

	void initVulkan();

	void mainLoop();
	
};


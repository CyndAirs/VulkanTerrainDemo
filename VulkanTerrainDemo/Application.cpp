#include "Application.h"



Application::Application()
{
}


Application::~Application()
{
	closeWindow();
}

void Application::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
}

void Application::closeWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::initVulkan()
{
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}


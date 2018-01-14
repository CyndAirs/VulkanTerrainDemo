#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shared.h"
#include "VulkanDeleter.h"
#include "QueueFamilyIndices.h"
#include "SwapChainDetails.h"
#include "FileRead.h"
#include "HeightMap.h"
#include "Vertex.h"
#include "CommandManager.h"
#include "SwapChainManager.h"
#include "BufferManager.h"
#include "RendererInfo.h"
#include "DeviceManager.h"
#include "Camera.hpp"

//#include "Window.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

#if BUILD_ENABLE_VULKAN_DEBUG
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
#endif // BUILD_ENABLE_VULKAN_DEBUG


/**
* The main controller class for the Vulkan Application
*/
class Renderer
{
public:
	/** 
	* Default contructor initialises object with default values for vertices and indces 
	*/
	Renderer();

	/** 
	* Contructor initialises object with given values for vertices and indices 
	* @param vertices vector with generated model vertices
	* @param indices vector with generated model indices
	* @param textureSource source of texture to be used for model
	*/
	Renderer(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string textureSource);

	~Renderer();

	/** 
	* Initialises Vulkan elements and window, starts main loop 
	* @param gui handle for gui window, so the statistics like fps can be returned
	*/
	void run(HWND gui);

	/**
	* Returns surface of the window 
	*/
	VkSurfaceKHR getSurface();

	/**
	* GLFW callback for keyboard input
	* @param window application window
	* @param key pressed key
	* @param scancode 
	* @param action type of key action
	* @param mods
	*/
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

	/**
	* GLFW callback for cursor position change
	* @param window application window
	* @param xpos x position of cursor
	* @param ypos y position of cursor
	*/
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	void ProcessClick(double xpos, double ypos);

	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);

	/**
	* Checks model rotation
	* @returns true if model is rotating to th left
	*/
	bool getRotateLeft();

	/**
	* Checks model rotation
	* @returns true if model is rotating to th right
	*/
	bool getRotateRight();

	/**
	* Sets model to rotate left (on key held)
	*/
	void setRotateLeft();

	/**
	* Sets model to rotate right (on key held)
	*/
	void setRotateRight();

	/**
	* Stops model from rotating (on key release)
	*/
	void stopRotation();

	/** 
	* Recreates swap chain and command buffers when the window is resized 
	* @param window application window
	* @param width new width of the window in pixels
	* @param height new height of the window in pixels
	*/
	static void onWindowResized(GLFWwindow * window, int width, int height);

private:

	/**
	* Vector containing models's vertices
	*/
	std::vector<Vertex> vertices;
	/**
	* Vector containing models's indices
	*/
	std::vector<uint32_t> indices;
	/**
	* String with image path of textures
	*/
	std::string textureSource;

	/** 
	* Initialises window for rendering 
	*/
	void initWindow();

	/** 
	* Closes window for rendering 
	*/
	void closeWindow();

	/** 
	* Initialises elements of Vulkan API
	*/
	void initVulkan();

	/** 
	* Main loop for rendering
	* @param gui handle for gui window, so the statistics like fps can be returned
	*/
	void mainLoop(HWND gui);

	/** 
	* Generates data for each frame
	* @param rotation rotation of the model calulated based on time in radians
	* @returns structure for uniform buffer
	*/
	UniformBufferObject generateUniformData(float rotationX, float rotationY);

	/**
	* Initialises instance 
	*/
	void initInstance();//

	/**
	* Checks if GLFW extensions are supported and loads them to instanceExtensions
	*/
	void setupGlfw();

	/** 
	* Initialises surface 
	*/
	void initSurface();

	/** 
	* Initialises semaphores
	*/
	void initSemaphores();	

	/** 
	* Draws a single frame 
	*/
	void drawFrame();

	/**
	* Window of Vulcan application
	*/
	GLFWwindow* window;

	/**
	* Vulkan instance
	*/
	VulkanDeleter<VkInstance> instance{ vkDestroyInstance };

	/**
	* Window surface
	*/
	VulkanDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };
	
	/**
	* DeviceManager containg informations about device
	*/
	DeviceManager deviceManager;

	/**
	* Callbak for errors
	*/
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};



	/**
	* Semaphore for checking if image is occupied
	*/
	VulkanDeleter<VkSemaphore> imageAvailableSemaphore{ deviceManager.device, vkDestroySemaphore };
	/**
	* Semaphore for checking if rendering is in progress
	*/
	VulkanDeleter<VkSemaphore> renderFinishedSemaphore{ deviceManager.device, vkDestroySemaphore };

	/**
	* BufferManager containing buffers, descriptor and related methods
	*/
	BufferManager bufferManager;

	/**
	* SwapChainManager containing swap chain, grapic pipeline and related methods
	*/
	SwapChainManager swapChain;

	/**
	* CommandManager for executing simple command buffers, contains command pool
	*/
	CommandManager * commandManager = new CommandManager();

	/**
	* Vector fo instance layers used
	*/
	std::vector<const char*> instanceLayers;

	/**
	* Vector for instance extensions used
	*/
	std::vector<const char*> instanceExtensions;

	/**
	* Flag to check if the model is rotating left
	*/
	bool rotateLeft = false;

	/**
	* Flag to check if the model is rotating right
	*/
	bool rotateRight = false;

	bool mousePressed = false;

	float forward, right, up;

	double xpos = 0, ypos = 0, lastXpos = 0, lastYpos = 0;

	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse = true;

	std::string test = std::string();
	std::wstring testTemp = std::wstring();

	enum AppState
	{
		FLOATING,
		EDITING
	};

	AppState state = FLOATING;

#if BUILD_ENABLE_VULKAN_DEBUG

	/** 
	* Sets up Validation Layer for debugging 
	*/
	void setupDebug();

	/** 
	* Initialises debug report callback
	*/
	void initDebug();

	VulkanDeleter<VkDebugReportCallbackEXT> debugReportCallback{ instance, DestroyDebugReportCallbackEXT };

#endif // BUILD_ENABLE_VULKAN_DEBUG
};

#endif // RENDERER_HPP
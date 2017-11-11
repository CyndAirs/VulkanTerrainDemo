#pragma once

#include "Shared.h"
#include "VulkanDeleter.h"
#include "QueueFamilyIndices.h"
#include "Vertex.h"

/**
* Struct containing information about application
*/
struct RendererInfo
{
	/** 
	* Vulkan instance handle
	*/
	VkInstance instance;

	/**
	* Windows suface handle
	*/
	VkSurfaceKHR surface;

	/**
	* Logical device handle
	*/
	VkDevice device;

	/**
	* Indices of queue families
	*/
	QueueFamilyIndices * queueFamilyIndices = nullptr;
	/**
	* Handle for graphics queue
	*/
	VkQueue * graphicsQueue = VK_NULL_HANDLE;
	/**
	* Handle for present queue
	*/
	VkQueue * presentQueue = VK_NULL_HANDLE;

	/**
	* Physical device handle
	*/
	VkPhysicalDevice gpu = VK_NULL_HANDLE;

	/**
	* Terrain model vertices
	*/
	std::vector<Vertex> vertices;

	/**
	* Terrain model indices
	*/
	std::vector<uint32_t> indices;

};
#pragma once

#include "Shared.h"
#include "VulkanDeleter.h"
#include "QueueFamilyIndices.h"

#include <vector>
#include <set>

/**
* Class initialising and holding the handels for physical device, queues and logical device
*/
class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();



	/** 
	* Initialises logical device 
	* @param instance Vulkan instance
	* @param surface window surface
	*/
	void initDevice(VkInstance instance, VkSurfaceKHR surface);//


	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	VulkanDeleter<VkDevice> device{ vkDestroyDevice };//

	/** 
	* Getter for logical device 
	* @returns logical device
	*/
	VkDevice getDevice();

	/** 
	* Getter for physical device 
	* @returns physical device
	*/
	VkPhysicalDevice getPhysicalDevice();

	/**
	* Getter for indices of device's queue families
	* @returns indices of device's queue families
	*/
	QueueFamilyIndices * getQueueFamilyIndices();

	/**
	* Getter for graphics queue
	* @returns address of graphics queue
	*/
	VkQueue & getGraphicsQueue();

	/**
	* Getter for present queue
	* @returns address of present queue
	*/
	VkQueue & getPresentQueue();

	/**
	* Getter for device extensions
	* @returns verctor containing available device extensions
	*/
	std::vector<const char*> getDeviceeExtensions();


private:

	/**
	* Structure for properties of physical device
	*/
	VkPhysicalDeviceProperties gpuProperties = {};
	/**
	* Handler for physical device
	*/
	VkPhysicalDevice gpu = VK_NULL_HANDLE;//

	/**
	* Indices for of device's queue families
	*/
	QueueFamilyIndices * queueFamilyIndices = nullptr;
	/**
	* Handler for graphics queue
	*/
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	/**
	* Handler for present queue
	*/
	VkQueue presentQueue = VK_NULL_HANDLE;

	/**
	* Vector with available device extensions
	*/
	std::vector<const char*> deviceExtensions;

};


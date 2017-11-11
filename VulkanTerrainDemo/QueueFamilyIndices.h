#pragma once
#include "Shared.h"

#include <vector>

/**
* Class for holding and managin the indices for queeu families.
* Mainly used for checking queues on the physical device.
*/
class QueueFamilyIndices
{
public:
	
	/**
	* Deafault constructor initialises both families as -1
	*/
	QueueFamilyIndices();

	/** 
	* Initialises the list of queue family indices by getting queue properties from graphics device and window surface 
	* @param device logical device handle
	* @param surface window surface
	*/
	QueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);

	~QueueFamilyIndices();

	/**
	* Getter for graphics queue family 
	* returns queue family numer
	*/
	int getGraphicsFamily();

	/**
	* Getter for present queue family
	* returns queue family numer
	*/
	int getPresentFamily();

private:

	/**
	* Index of graphics queue family
	*/
	int graphicsFamily;
	/**
	* Index of present queue family
	*/
	int presentFamily;

	/** 
	* Checks if all queue families have been initialised 
	* returns true if queue families' indices are not equal to 0
	*/
	bool isComplete();
};


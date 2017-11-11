#pragma once

#include "Shared.h"
#include <vector>
#include <algorithm>
#include <limits>


/**
* Class used to collect data for SwapChain
*/
class SwapChainDetails
{
public:
	/** 
	* Constructor
	* @param device physical device handle
	* @param surface window surface handle
	*/
	SwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
	/**
	* Chooses format of surface
	* @returns format
	*/
	VkSurfaceFormatKHR chooseSwapSurfaceFormat();
	/**
	* Chooses present mode of surface
	* @returns prsent mode
	*/
	VkPresentModeKHR chooseSwapPresentMode();
	/**
	* Chooses the extent of the swap chain
	* @returns Handle for extent object
	*/
	VkExtent2D chooseSwapExtent();

	~SwapChainDetails();
	/**
	* Return capabiliies of the suface
	* @returns capabilities of window suface
	*/
	VkSurfaceCapabilitiesKHR getCapabilities();
	/**
	* Return formats of surface
	* @returns vector of formats
	*/
	std::vector<VkSurfaceFormatKHR> getFormats();
	/**
	* Return present modes of suface
	* @returns present modes of suface
	*/
	std::vector<VkPresentModeKHR> getPresentModes();



private:

	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


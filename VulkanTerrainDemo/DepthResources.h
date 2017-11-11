#pragma once
#include "ImageResources.h"

/**
* Class used for handling depth images
*/
class DepthResources : public ImageResources
{
public:
	/**
	* Checks the device for depth image format support
	* @param renderer info about application
	* @param commandManager pointer to command manager
	* @param swapChainExtent extents size of swap chain
	*/
	DepthResources(RendererInfo renderer, CommandManager * commandManager, VkExtent2D swapChainExtent);
	~DepthResources();

	/** 
	* Initialises depth image, checks for supported format on device and transitions layout of the image 
	*/
	void init();

	/**
	* Checks device for available depth formats and return them 
	* @returns available format
	*/
	VkFormat findDepthFormat();

private:

	/**
	* Size of the swap chain
	*/
	VkExtent2D swapChainExtent;

	/**
	* Checks the device for depth image format support
	* @param candidates list of formats to check
	* @param tiling image tiling
	* @param features flags for image features
	* @returns available format
	*/
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	
};


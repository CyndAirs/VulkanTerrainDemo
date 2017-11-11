#pragma once

#ifndef IMAGE_RESOURCES_HPP
#define IMAGE_RESOURCES_HPP

#include "Shared.h"
#include "VulkanDeleter.h"
#include "FindMemoryType.h"
#include "RendererInfo.h"
#include "CommandManager.h"

/**
* Class for handling images
*/
class ImageResources
{
public:
	/** 
	* Constructor, saves Renderer object and a pointer to commandManager inside class
	* @param renderer info about application
	* @param commandManager command manager
	*/
	ImageResources(RendererInfo renderer, CommandManager * commandManager);

	/** 
	*Destructor
	*/
	~ImageResources();

	/** 
	* Getter for imageViews 
	* @returns Handle fo image views
	*/
	VkImageView getImageView();

	/**
	* Virtual void for initialising image resources
	*/
	virtual void init() {};

	/** 
	* Initialises the image view of given format and flags 
	* @param image image for view
	* @param format format of the image
	* @param aspectFlags aspect flags of the image
	* @param imageView address for imageView handler
	*/
	void initImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanDeleter<VkImageView>& imageView);

	/** Copies image of given width and height to other image using command buffers 
	* @param srcImage source image
	* @param dstImage destination image
	* @param width image width
	* @param height image height
	*/
	void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
	
protected:

	/** 
	* Initialises the resource class for a 2D image 
	* @param width width of the image
	* @param height height of the image
	* @param format format of the image
	* @param tiling tiling of the image
	* @param usage image's usage flags
	* @param properties flags for memory properties
	* @param image image itself
	* @param imageMemory address of allocated memory fo rthe image
	*/
	void initImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags  usage, VkMemoryPropertyFlags properties, VulkanDeleter<VkImage>& image, VulkanDeleter<VkDeviceMemory>& imageMemory);
	
	/** 
	* Transitions image's layout and configures it to use in the pipeline using command buffers 
	* @param image the image to transition
	* @param format format of the image
	* @param oldLayout current layout of the image
	* @param newLayout desired layout of the flags
	*/
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	/** 
	* Checks if the image has stencil component 
	* returns true if the image has stencil component 
	*/
	bool hasStencilComponent(VkFormat format);

	/**
	* Handler for the image
	*/
	VulkanDeleter<VkImage> image{ renderer.device, vkDestroyImage };
	/**
	* Handler for the memory allocated for the image
	*/
	VulkanDeleter<VkDeviceMemory> imageMemory{ renderer.device, vkFreeMemory };
	/**
	* Handler for the image view
	*/
	VulkanDeleter<VkImageView> imageView{ renderer.device, vkDestroyImageView };

	/**
	* Info about application
	*/
	RendererInfo renderer;

	/**
	* Pointer to command manager
	*/
	CommandManager * commandManager;

};

#endif //IMAGE_RESOURCES_HPP


#pragma once
#include "ImageResources.h"
#include "Image.h"
#include <string>
/**
* Class fro managing textures
*/
class TextureResources : public ImageResources
{
public:
	/**
	* Initialises object from data from application and command manager
	* @param renderer information about renderer class
	* @param commandManager pointer to command manager
	*/
	TextureResources(RendererInfo renderer, CommandManager * commandManager);
	~TextureResources();

	/** 
	* Initialises Texture resourses from default source 
	*/
	void init();

	/** 
	* Initialises Texture resourses from given source 
	* @param textureSource path to image file
	*/
	void init(std::string textureSource);

	/** 
	* Initialises texture sampler for texture image 
	*/
	void initTextureSampler();

	/**
	* Returns image info for descriptors 
	* @returns image info for descriptors 
	*/
	VkDescriptorImageInfo getImageInfo();

private:

	/**
	* Sampler for image
	*/
	VulkanDeleter<VkSampler> sampler{ renderer.device, vkDestroySampler };
};


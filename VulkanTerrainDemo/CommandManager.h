#pragma once
#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP
#include "Shared.h"
#include "RendererInfo.h"

/**
* Class for managing command pool and single use commands
*/
class CommandManager
{
public:
	CommandManager();
	~CommandManager();

	/** 
	* Method returning a command buffer for one time use
	* @returns command buffer
	*/
	VkCommandBuffer beginSingleCommand();

	/** 
	* Takes nad flushes sigle time command buffer
	* @param commandBuffer command buffer to flush
	*/
	void endSingleCommand(VkCommandBuffer commandBuffer);

	/** 
	* Getter for command pool 
	* @returns command pool
	*/
	VkCommandPool getCommandPool();

	/** 
	* Initialises the class and the command pool
	* @param renderer info about application
	*/
	void init(RendererInfo renderer);


	/**
	* Initialises the command pool
	*/
	void initCommandPool();


private:

	VulkanDeleter<VkCommandPool> commandPool{ renderer.device, vkDestroyCommandPool };


	RendererInfo renderer;
};

#endif COMMAND_MANAGER_HPP


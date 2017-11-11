#pragma once

#include "Shared.h"
#include "VulkanDeleter.h"
#include "SwapChainDetails.h"
#include "FileRead.h"
#include "Vertex.h"
#include "DepthResources.h"
#include "RendererInfo.h"
#include <vector>
#include <array>

/**
* Class for initialising and holding swap chain, render pass, depth image and graphic pipeline.
*/
class SwapChainManager
{
public:
	/** 
	* Initialises swap chain as well as any other objects organized inside SwapChainManager obeject 
	* @param renderer information about renderer class
	* @param commandManager pointer to command manager
	*/
	void init(RendererInfo renderer, CommandManager * commandManager);
	/** 
	* Default constructor 
	*/
	SwapChainManager();
	~SwapChainManager();
	/** 
	* Recreates swap chain in case of the current one being outdated (eg. when window is resised)
	*/
	void recreateSwapChain();
	/**
	* Holds the information about hte format of swap chain images
	*/
	VkFormat swapChainImageFormat;
	/**
	* Size of the swap chain
	*/
	VkExtent2D swapChainExtent;
	/**
	* Handle for Render Pass
	*/
	VulkanDeleter<VkRenderPass> renderPass{ renderer.device, vkDestroyRenderPass };
	/**
	* Handle for the layout of descriptors
	*/
	VulkanDeleter<VkDescriptorSetLayout> descriptorSetLayout{ renderer.device, vkDestroyDescriptorSetLayout };
	/**
	* Handle for the layoy of graphical pipeline
	*/
	VulkanDeleter<VkPipelineLayout> pipelineLayout{ renderer.device, vkDestroyPipelineLayout };
	/**
	* Handle for the graphics pipileine
	*/
	VulkanDeleter<VkPipeline> graphicsPipeline{ renderer.device, vkDestroyPipeline };
	/**
	* Hanlde for the swpa chain
	*/
	VulkanDeleter<VkSwapchainKHR> swapChain{ renderer.device, vkDestroySwapchainKHR };
	/**
	* Vector with the views of the swap chain images
	*/
	std::vector<VulkanDeleter<VkImageView>> swapChainImageViews;
	/**
	* Vector with the frame buffers for the swap chain
	*/
	std::vector<VulkanDeleter<VkFramebuffer>> swapChainFramebuffers;
	/**
	* Vector holding swap chain images, which are updated and send to be rendered on the screen
	*/
	std::vector<VkImage> swapChainImages;


private:
	
	/** 
	* Initialises image views (image storage) for the swap chain 
	*/
	void initImageViews();

	/**
	* Initialises frame buffer to be used by swap chain 
	*/
	void initFrameBuffers();

	/**
	* Initialises descriptor layout for uniform buffers and samplers 
	*/
	void initDescriptorSetLayout();

	/** 
	* Initialises and configures the graphic pipeline, loads vertex and fragment shaders, configures assembly and default viewport and scissors 
	*/
	void initGraphicsPipeline();

	/** 
	* Initialises shader module for given shader code 
	* @param code compiled code of the shade
	* @param shaderModule addres of the memory space allocated for the module
	*/
	void initShaderModule(const std::vector<char>& code, VulkanDeleter<VkShaderModule>& shaderModule);

	/** 
	* Initiliases VkRenderPass object, whick will allow to send images between vulkan objects 
	*/
	void initRenderPass();

	/** 
	* Initialises resourses required for Depth Image 
	* @param commandManager pointer the the command manager object in order to use single time commands
	*/
	void initDepthResources(CommandManager * commandManager);

	/**
	* Information about the application
	*/
	RendererInfo renderer;

	/**
	* Pointer to object generating details for swap chain configuration
	*/
	SwapChainDetails * swapChainDetails = nullptr;

	/**
	* Pointer to object managing the depth image
	*/
	DepthResources * depthResources;

	/**
	* Pointer to object managing commands
	*/
	CommandManager * commandManager;
	
};


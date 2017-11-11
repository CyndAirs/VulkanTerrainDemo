#pragma once
#include "Shared.h"
#include "VulkanDeleter.h"
#include "RendererInfo.h"
#include "SwapChainManager.h"
#include "TextureResources.h"

#include<vector>
#include <chrono>

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 lightPosition;
};

/*
* Class used for initialising and managing buffers
*/
class BufferManager
{
public:
	BufferManager();
	~BufferManager();

	/** 
	 * Initialises buffers and descriptors
	 * @param renderer information about renderer class
	 * @param swapChain pointer to swap chain manager
	 * @param commandManager pointer to command manager
	 * @param textureSource path to source image for textures
	 */
	void init(RendererInfo renderer, SwapChainManager * swapChain, CommandManager * commandManager, std::string textureSource);

	/** 
	 * Copies one buffer to another, method is used mainly to copy buffer into GPU memory
	 * @param srcBuffer source buffer
	 * @param dstBuffer destination buffer
	 * @param size size of the buffer
	 */
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	/** 
	 * Getter for command buffer for given image
	 * @param imageIndex index of given image
	 * @returns command buffer
	 */
	VkCommandBuffer * getCommandBuffer(int imageIndex);


	/** 
	 * Initialises given buffer 
	 * @param size size of the buffer
	 * @param bufferUsage usage flags for buffer
	 * @param memoryProperties flags for memory properties
	 * @param buffer buffer address
	 * @param bufferMemory allocated memory for the buffer
	 */
	void initBuffer(VkDeviceSize size, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags memoryProperties, VulkanDeleter<VkBuffer>& buffer, VulkanDeleter<VkDeviceMemory>& bufferMemory);

	
	/**
	 * Initialises vertex buffer
	 */
	void initVertexBuffer();

	/**
	 * Initialises index buffer
	 */
	void initIndexBuffer();
	
	/**
	 * Initialises uniform buffer, which contains current transformations of the object
	 */
	void initUniformBuffer();

	/**
	 * Initialises descriptor pool
	 */
	void initDescriptorPool();

	/** 
	 * Initialises descriptor set for model 
	 */
	void initDescriptorSet();

	/** 
	 * initialises command buffers 
	 */
	void initCommandBuffers();

	/** 
	 * Updates uniform buffer with given uniform buffer objects containing data for shaders
	 * @param ubo contains data for shaders
	 */
	void updateUniformBuffer(UniformBufferObject ubo);

private:

	/**
	* Vector of command bufers for all images
	*/
	std::vector<VkCommandBuffer> commandBuffers;

	/**
	* Buffer for vertices
	*/
	VulkanDeleter<VkBuffer> vertexBuffer{ renderer.device, vkDestroyBuffer };
	/**
	* Memory allocated for vertex buffer
	*/
	VulkanDeleter<VkDeviceMemory> vertexBufferMemory{ renderer.device, vkFreeMemory };

	/**
	* Buffer for indices
	*/
	VulkanDeleter<VkBuffer> indexBuffer{ renderer.device, vkDestroyBuffer };
	/**
	* Memory allocated for vertex buffer
	*/
	VulkanDeleter<VkDeviceMemory> indexBufferMemory{ renderer.device, vkFreeMemory };

	/**
	* Buffer for uniform buffer staging
	*/
	VulkanDeleter<VkBuffer> uniformStagingBuffer{ renderer.device, vkDestroyBuffer };
	/**
	* Memory allocated for buffer for uniform buffer staging
	*/
	VulkanDeleter<VkDeviceMemory> uniformStagingBufferMemory{ renderer.device, vkFreeMemory };
	/**
	* Uniform buffer
	*/
	VulkanDeleter<VkBuffer> uniformBuffer{ renderer.device, vkDestroyBuffer };
	/**
	* Memory alloacted for uniform buffer
	*/
	VulkanDeleter<VkDeviceMemory> uniformBufferMemory{ renderer.device, vkFreeMemory };

	/**
	* Descriptor pool for descriptors for shaders
	*/
	VulkanDeleter<VkDescriptorPool> descriptorPool{ renderer.device, vkDestroyDescriptorPool };
	/**
	* Set of descriptors for shaders
	*/
	VkDescriptorSet descriptorSet;

	/**
	* Info on renderer class
	*/
	RendererInfo renderer;

	/**
	* Pointer to swap chain manager
	*/
	SwapChainManager * swapChain;

	/**
	* Pointer to swap chain manager
	*/
	TextureResources * textureResources;

	/**
	* Pointer to command manager
	*/
	CommandManager * commandManager;
};


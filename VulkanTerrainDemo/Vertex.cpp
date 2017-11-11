#include "Vertex.h"


VkVertexInputBindingDescription Vertex::getInputBindingDescription()
{
	VkVertexInputBindingDescription inputBindingDescription = {};
	inputBindingDescription.binding = 0;
	inputBindingDescription.stride = sizeof(Vertex);
	inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return inputBindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4> Vertex::getInputAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 4> inputAttributeDescriptions = {};

	inputAttributeDescriptions[0].binding = 0;
	inputAttributeDescriptions[0].location = 0;
	inputAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	inputAttributeDescriptions[0].offset = offsetof(Vertex, pos);

	inputAttributeDescriptions[1].binding = 0;
	inputAttributeDescriptions[1].location = 1;
	inputAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	inputAttributeDescriptions[1].offset = offsetof(Vertex, color);

	inputAttributeDescriptions[2].binding = 0;
	inputAttributeDescriptions[2].location = 2;
	inputAttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	inputAttributeDescriptions[2].offset = offsetof(Vertex, texPos);

	inputAttributeDescriptions[3].binding = 0;
	inputAttributeDescriptions[3].location = 3;
	inputAttributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	inputAttributeDescriptions[3].offset = offsetof(Vertex, normal);

	return inputAttributeDescriptions;
}

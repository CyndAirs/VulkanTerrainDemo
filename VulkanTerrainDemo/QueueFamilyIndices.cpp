#include "QueueFamilyIndices.h"



QueueFamilyIndices::QueueFamilyIndices() : graphicsFamily(-1), presentFamily(-1){}

QueueFamilyIndices::QueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface) {

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	
	VkBool32 presentSupport;
	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamily = i;
		}

		presentSupport = true;

		ErrorCheck(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport));

		if (queueFamily.queueCount > 0 && presentSupport) {
			presentFamily = i;
		}

		if (isComplete()) {
			break;
		}

		i++;
	}

	if (!isComplete()) {
		throw std::runtime_error( "Vulkan Error: Queue family supporting graphics not found");
		std::exit(-1);
	}
}


QueueFamilyIndices::~QueueFamilyIndices(){}

 int QueueFamilyIndices::getGraphicsFamily()
{
	return graphicsFamily;
}

 int QueueFamilyIndices::getPresentFamily()
{
	return presentFamily;
}

bool QueueFamilyIndices::isComplete()
{
	return (graphicsFamily >= 0) && (presentFamily >= 0);
}
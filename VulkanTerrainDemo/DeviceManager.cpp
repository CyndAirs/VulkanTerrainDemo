#include "DeviceManager.h"



DeviceManager::DeviceManager()
{
}


DeviceManager::~DeviceManager()
{
}


void DeviceManager::initDevice(VkInstance instance, VkSurfaceKHR surface)
{
	uint32_t gpuCount = 0;
	vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
	std::vector<VkPhysicalDevice> gpuList(gpuCount);
	vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList.data());
	gpu = gpuList[0];
	vkGetPhysicalDeviceProperties(gpu, &gpuProperties);
	if (!checkDeviceExtensionSupport(gpu)) {
		throw std::runtime_error("Vulkan Error: Some of required extensions are not supported.");
		std::exit(-1);
	}
	deviceExtensions = DEVICE_EXTENSIONS;

	queueFamilyIndices = new QueueFamilyIndices(gpu, surface);

	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
	std::set<int> uniqueQueueFamilies = { queueFamilyIndices->getGraphicsFamily(), queueFamilyIndices->getPresentFamily() };

	float queuePriorities[]{ 1.0f };
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = queuePriorities;
		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();

	deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	ErrorCheck(vkCreateDevice(gpu, &deviceCreateInfo, nullptr, device.replace()));

	vkGetDeviceQueue(device, queueFamilyIndices->getGraphicsFamily(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueFamilyIndices->getPresentFamily(), 0, &presentQueue);
}



VkDevice DeviceManager::getDevice()
{
	return device;
}

VkPhysicalDevice DeviceManager::getPhysicalDevice()
{
	return gpu;
}

QueueFamilyIndices * DeviceManager::getQueueFamilyIndices()
{
	return queueFamilyIndices;
}

VkQueue & DeviceManager::getGraphicsQueue()
{
	return graphicsQueue;
}

VkQueue & DeviceManager::getPresentQueue()
{
	return presentQueue;
}

std::vector<const char*> DeviceManager::getDeviceeExtensions()
{
	return deviceExtensions;
}

bool DeviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS);

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
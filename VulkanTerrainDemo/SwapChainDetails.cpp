#include "SwapChainDetails.h"



SwapChainDetails::SwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{

	ErrorCheck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities));

	uint32_t formatCount;
	ErrorCheck(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr));

	if (formatCount != 0) {
		formats.resize(formatCount);
		ErrorCheck(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data()));
	}

	uint32_t presentModeCount;
	ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr));

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data()));
	}

	if (formats.empty() || presentModes.empty()) {

		throw std::runtime_error("Vulkan Error: Swap Chain not fully supported.");
		std::exit(-1);
	}

}

VkSurfaceFormatKHR SwapChainDetails::chooseSwapSurfaceFormat() {
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& format : formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}

	return formats[0];
}

VkPresentModeKHR SwapChainDetails::chooseSwapPresentMode() {
	for (const auto& presentMode : presentModes) {
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return presentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainDetails::chooseSwapExtent() {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { WINDOW_WIDTH, WINDOW_HEIGHT };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}


SwapChainDetails::~SwapChainDetails()
{
}

VkSurfaceCapabilitiesKHR SwapChainDetails::getCapabilities()
{
	return capabilities;
}

std::vector<VkSurfaceFormatKHR> SwapChainDetails::getFormats()
{
	return formats;
}

std::vector<VkPresentModeKHR> SwapChainDetails::getPresentModes()
{
	return presentModes;
}

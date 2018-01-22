#include "Shared.h"



uint32_t findMemoryType(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
	return 1;
}

std::wstring get_utf16(const std::string &str, int codepage)
{
	if (str.empty()) return std::wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG

#ifdef WIN32
void ErrorCheck(VkResult result) {
	if (result < 0) {
		switch (result)
		{
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			MessageBox(NULL, L"VK_ERROR_OUT_OF_HOST_MEMORY", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			MessageBox(NULL, L"VK_ERROR_OUT_OF_DEVICE_MEMORY", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			MessageBox(NULL, L"VK_ERROR_INITIALIZATION_FAILED", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_DEVICE_LOST:
			MessageBox(NULL, L"VK_ERROR_DEVICE_LOST", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			MessageBox(NULL, L"VK_ERROR_MEMORY_MAP_FAILED", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			MessageBox(NULL, L"VK_ERROR_LAYER_NOT_PRESENT", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			MessageBox(NULL, L"VK_ERROR_EXTENSION_NOT_PRESENT", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			MessageBox(NULL, L"VK_ERROR_FEATURE_NOT_PRESENT", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			MessageBox(NULL, L"VK_ERROR_INCOMPATIBLE_DRIVER", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			MessageBox(NULL, L"VK_ERROR_TOO_MANY_OBJECTS", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			MessageBox(NULL, L"VK_ERROR_FORMAT_NOT_SUPPORTED", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_FRAGMENTED_POOL:
			MessageBox(NULL, L"VK_ERROR_FRAGMENTED_POOL", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			MessageBox(NULL, L"VK_ERROR_SURFACE_LOST_KHR", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			MessageBox(NULL, L"VK_ERROR_NATIVE_WINDOW_IN_USE_KHR", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_SUBOPTIMAL_KHR:
			MessageBox(NULL, L"VK_SUBOPTIMAL_KHR", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			MessageBox(NULL, L"VK_ERROR_OUT_OF_DATE_KHR", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			MessageBox(NULL, L"VK_ERROR_INCOMPATIBLE_DISPLAY_KHR", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			MessageBox(NULL, L"VK_ERROR_VALIDATION_FAILED_EXT", L"Error", MB_ICONERROR | MB_OK);
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			MessageBox(NULL, L"VK_ERROR_INVALID_SHADER_NV", L"Error", MB_ICONERROR | MB_OK);
			break;
		default:
			break;
		}
	}

}
#else
void ErrorCheck(VkResult result) {
	if (result < 0) {
		switch (result)
		{
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
			break;
		case VK_ERROR_DEVICE_LOST:
			std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
			break;
		case VK_ERROR_FRAGMENTED_POOL:
			std::cout << "VK_ERROR_FRAGMENTED_POOL" << std::endl;
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
			break;
		case VK_SUBOPTIMAL_KHR:
			std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			std::cout << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			std::cout << "VK_ERROR_INVALID_SHADER_NV" << std::endl;
			break;
		default:
			break;
		}
	}

}
#endif

#else

void ErrorCheck(VkResult result) {}

#endif
#pragma once

#ifdef _WIN32

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>

#else

#error Operating system not suppoerted

#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


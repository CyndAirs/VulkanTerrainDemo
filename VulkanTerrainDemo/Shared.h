#pragma once

#ifndef SHARED_H
#define SHARED_H

#include "Platform.h"
#include "Settings.h"

#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <functional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** 
* Check graphical device's memory properties and returns suitable memory type 
* @param gpu handle for physical device
* @param typeFilter seeked type
* @param properties type properties flags
* @returns type index
*/
uint32_t findMemoryType(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties);

std::wstring get_utf16(const std::string &str, int codepage);
/** 
* Checks results of Vulkan functions and interprets them as errors when nessesary 
* @param result value returned from vulkan operation
*/
void ErrorCheck(VkResult result);


#endif // !SHARED_H




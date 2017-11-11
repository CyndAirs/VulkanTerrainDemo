#pragma once
#ifndef VULKAN_DELETER
#define VULKAN_DELETER
#include "Shared.h"

/**
* Template used to automatically delete Vulkan objects when the program ends. 
* Since vulkan handles are deleted with set number of functions you can geralise them in such template.
*/
template <typename T>
class VulkanDeleter {
public:
	/**
	* Constructor
	*/
	VulkanDeleter() : VulkanDeleter([](T, VkAllocationCallbacks*) {}) {}

	/**
	* Direct deleter of the object
	*/
	VulkanDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef);
	/**
	* Deleter of object requiring instance for deletion
	*/
	VulkanDeleter(const VulkanDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef);
	/**
	* Deleter of object requiring logical device for deletion, with logical device using this template
	*/
	VulkanDeleter(const VulkanDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef);
	/**
	* Deleter of object requiring logical device for deletion, with logical devie not using this template
	*/
	VulkanDeleter(const VkDevice& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef);

	~VulkanDeleter();

	/**
	* Operator for returning the address of the object
	*/
	const T* operator &() const;
	/**
	* Function for replacing the object
	*/
	T* replace();
	/**
	* Operator for returning object using template as object of its class
	*/
	operator T() const;
	/**
	* Overload of assigment operator to use the object instead of template
	*/
	void operator=(T rhs);

	/**
	* Operator checking if object is equal to another object of its type
	* @param rhs object to compare
	* @returns true if objects are equal
	*/
	template<typename V>
	bool operator==(V rhs);

private:
	/**
	* Hold the object itself
	*/
	T object{ VK_NULL_HANDLE };
	/**
	* Hold the deleting function
	*/
	std::function<void(T)> deleter;

	/**
	* Deltes object
	*/
	void cleanup();
};

template<typename T>
inline VulkanDeleter<T>::VulkanDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [=](T obj) { deletef(obj, nullptr); };
}

template<typename T>
VulkanDeleter<T>::VulkanDeleter(const VulkanDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
}

template<typename T>
VulkanDeleter<T>::VulkanDeleter(const VulkanDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
}

template<typename T>
VulkanDeleter<T>::VulkanDeleter(const VkDevice& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
}

template<typename T>
VulkanDeleter<T>::~VulkanDeleter()
{
	cleanup();
}

template<typename T>
const T * VulkanDeleter<T>::operator&() const
{
	return &object;
}

template<typename T>
T * VulkanDeleter<T>::replace()
{
	cleanup();
	return &object;
}

template<typename T>
VulkanDeleter<T>::operator T() const
{
	return object;
}

template<typename T>
void VulkanDeleter<T>::operator=(T rhs)
{
	if (rhs != object) {
		cleanup();
		object = rhs;
	}
}

template<typename T>
void VulkanDeleter<T>::cleanup()
{
	if (object != VK_NULL_HANDLE) {
		deleter(object);
	}
	object = VK_NULL_HANDLE;
}

template<typename T>
template<typename V>
inline bool VulkanDeleter<T>::operator==(V rhs)
{
	return object == T(rhs);
}

#endif //VULKAN_DELETER



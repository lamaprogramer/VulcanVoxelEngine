#pragma once
#include "VulkanDebugMessenger.h"
#include "VulkanPhysicalDevice.h"
#include <set>

class VulkanLogicalDevice {
public:
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VulkanLogicalDevice();
	VulkanLogicalDevice(VulkanPhysicalDevice physicalDevice, VulkanSurface surface, std::vector<const char*> deviceExtensions, 
		std::vector<const char*> validationLayers, bool enableValidationLayers);
};
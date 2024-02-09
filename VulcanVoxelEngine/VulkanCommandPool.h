#pragma once
#include "VulkanLogicalDevice.h"

class VulkanCommandPool {
public:
	VkCommandPool commandPool;

	VulkanCommandPool();
	VulkanCommandPool(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSurface surface);
};
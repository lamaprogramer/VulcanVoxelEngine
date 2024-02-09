#pragma once
#include "VulkanLogicalDevice.h"

class VulkanDescriptorPool {
public:
	VkDescriptorPool descriptorPool;

	VulkanDescriptorPool();
	VulkanDescriptorPool(VulkanLogicalDevice device, int maxFramesInFlight);
};
#pragma once
#include "VulkanLogicalDevice.h"

class VulkanDescriptorPool {
public:
	VkDescriptorPool descriptorPool;

	VulkanDescriptorPool();
	VulkanDescriptorPool(VulkanLogicalDevice device, int maxFramesInFlight);

	VkDescriptorPoolSize creatPoolSize(VkDescriptorType type, uint32_t descriptorCount);
};
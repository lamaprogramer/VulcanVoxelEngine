#pragma once
#include "VulkanLogicalDevice.h"

class VulkanImageSampler {
public:
	VkSampler textureSampler;

	VulkanImageSampler();
	VulkanImageSampler(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device);

	void destroy(VulkanLogicalDevice device);
};
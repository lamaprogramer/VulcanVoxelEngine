#pragma once
#include "VulkanLogicalDevice.h"

class VulkanImageSampler {
public:
	VkSampler textureSampler;

	VulkanImageSampler();
	VulkanImageSampler(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device);
};
#pragma once
#include "VulkanLogicalDevice.h"

class VulkanDescriptorSetLayout {
public:
	VkDescriptorSetLayout descriptorSetLayout;

	VulkanDescriptorSetLayout();
	VulkanDescriptorSetLayout(VulkanLogicalDevice device);
};
#pragma once
#include "VulkanLogicalDevice.h"

class VulkanDescriptorSetLayout {
public:
	VkDescriptorSetLayout descriptorSetLayout;

	VulkanDescriptorSetLayout();
	VulkanDescriptorSetLayout(VulkanLogicalDevice device);

	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags flags, uint32_t binding);
};
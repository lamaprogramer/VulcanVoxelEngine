#pragma once
#include "VulkanLogicalDevice.h"

class VulkanDescriptorSetLayout {
public:
	VkDescriptorSetLayout descriptorSetLayout;

	VulkanDescriptorSetLayout();
	VulkanDescriptorSetLayout(VulkanLogicalDevice device, std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings);

	static VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags flags, uint32_t binding);
};
#pragma once
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanUniformBuffer.h"

class VulkanDescriptorSets {
public:
	std::vector<VkDescriptorSet> descriptorSets;

	VulkanDescriptorSets();
	VulkanDescriptorSets(VulkanLogicalDevice device, VulkanUniformBuffer uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout descriptorSetLayout, int maxFramesInFlight);
};
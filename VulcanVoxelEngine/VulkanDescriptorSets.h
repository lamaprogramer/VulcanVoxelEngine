#pragma once
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSet.h"
#include "Texture.h"
#include <map>

class VulkanDescriptorSets {
public:
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> globalDescriptorSets;


	VulkanDescriptorSets();
	VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout globalDescriptorSetLayout, int maxFramesInFlight);

	static void allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, std::vector<VkDescriptorSet> &descriptorSets, uint32_t descriptorCount);
	static VkDescriptorBufferInfo createDescriptorBufferInfo(VkBuffer buffer);
};
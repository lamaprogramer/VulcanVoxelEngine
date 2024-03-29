#pragma once
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanUniformBuffer.h"
#include "VulkanImageView.h"
#include "VulkanImageSampler.h"

class VulkanDescriptorSets {
public:
	std::vector<VkDescriptorSet> descriptorSets;

	VulkanDescriptorSets();
	VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout descriptorSetLayout, VulkanImageView textureImageView, VulkanImageSampler textureSampler, int maxFramesInFlight);

	void allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, std::vector<VkDescriptorSet> &descriptorSets, uint32_t descriptorCount);
	VkDescriptorBufferInfo createDescriptorBufferInfo(VkBuffer buffer);
	VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorBufferInfo info);
	VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorImageInfo info);
};
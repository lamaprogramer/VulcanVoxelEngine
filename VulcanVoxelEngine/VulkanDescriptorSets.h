#pragma once
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSet.h"
#include "Texture.h"
#include <map>

class VulkanDescriptorSets {
public:
	//std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> globalDescriptorSets;
	std::map<std::string, VkDescriptorSet> descriptorSets;


	VulkanDescriptorSets();
	VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout globalDescriptorSetLayout, VulkanDescriptorSetLayout descriptorSetLayout, std::map<std::string, Texture> textureImageViews, int maxFramesInFlight);

	static void allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, std::vector<VkDescriptorSet> &descriptorSets, uint32_t descriptorCount);
	static void allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, VkDescriptorSet& descriptorSets);
	static VkDescriptorBufferInfo createDescriptorBufferInfo(VkBuffer buffer);
	static VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorBufferInfo info);
	static VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorImageInfo info);
};
#pragma once

#include "VulkanImageView.h"
#include "VulkanImageSampler.h"

class VulkanDescriptorSet {
public:
	static VkWriteDescriptorSet writeImageDescriptor(VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorImageInfo info);
	static VkWriteDescriptorSet writeUniformBufferDescriptor(VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorBufferInfo info);
};
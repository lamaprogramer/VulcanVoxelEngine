#pragma once
#include "VulkanBuffer.h"

class VulkanInstanceBuffer: public VulkanBuffer {
public:
	VulkanInstanceBuffer();
	VulkanInstanceBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize);
	VulkanInstanceBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize, VkMemoryPropertyFlags properties);
};
#pragma once

#include "VulkanBuffer.h"

class VulkanIndexBuffer: public VulkanBuffer {
public:
	VulkanIndexBuffer();
	VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize);
};
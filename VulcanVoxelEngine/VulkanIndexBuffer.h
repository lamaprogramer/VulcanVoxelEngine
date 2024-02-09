#pragma once

#include "VulkanBufferUtil.h"

class VulkanIndexBuffer {
public:
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VulkanIndexBuffer();
	VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<uint16_t> indices);
};
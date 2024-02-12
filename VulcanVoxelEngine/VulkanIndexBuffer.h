#pragma once

#include "VulkanBuffer.h"

class VulkanIndexBuffer: public VulkanBuffer {
public:
	VulkanIndexBuffer();
	VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<uint16_t> indices);
};
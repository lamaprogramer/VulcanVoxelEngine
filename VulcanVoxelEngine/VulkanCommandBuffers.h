#pragma once
#include "VulkanCommandPool.h"

class VulkanCommandBuffers {
public:
	std::vector<VkCommandBuffer> commandBuffers;

	VulkanCommandBuffers();
	VulkanCommandBuffers(VulkanLogicalDevice device, VulkanCommandPool commandPool, int maxFramesInFlight);
};
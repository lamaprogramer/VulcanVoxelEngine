#pragma once
#include "VulkanSwapChain.h"

class VulkanImageViews {
public:
	std::vector<VkImageView> swapChainImageViews;

	VulkanImageViews();
	VulkanImageViews(VulkanLogicalDevice device, VulkanSwapChain swapChain);
};
#pragma once
#include "VulkanSwapChain.h"

class VulkanRenderPass {
public:
	VkRenderPass renderPass;

	VulkanRenderPass();
	VulkanRenderPass(VulkanLogicalDevice device, VulkanSwapChain swapChain);
};
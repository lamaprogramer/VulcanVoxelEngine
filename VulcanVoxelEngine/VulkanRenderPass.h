#pragma once
#include "VulkanSwapChain.h"
#include "VulkanDepthImage.h"

class VulkanRenderPass {
public:
	VkRenderPass renderPass;

	VulkanRenderPass();
	VulkanRenderPass(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSwapChain swapChain);
};
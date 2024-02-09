#pragma once
#include "VulkanImageViews.h"
#include "VulkanRenderPass.h"

class VulkanFrameBuffers {
public:
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VulkanFrameBuffers();
	VulkanFrameBuffers(VulkanLogicalDevice device, VulkanSwapChain swapChain, VulkanImageViews swapChainImageViews, VulkanRenderPass renderPass);
};
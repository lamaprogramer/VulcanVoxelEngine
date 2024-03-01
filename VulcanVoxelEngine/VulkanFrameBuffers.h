#pragma once
#include "VulkanImageViews.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"

class VulkanFrameBuffers {
public:
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VulkanFrameBuffers();
	VulkanFrameBuffers(VulkanLogicalDevice device, VulkanSwapChain swapChain, std::vector<VulkanImageView> swapChainImageViews, VulkanImageView depthImageView, VulkanRenderPass renderPass);
};
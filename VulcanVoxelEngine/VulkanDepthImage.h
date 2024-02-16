#pragma once
#include "VulkanImage.h"
#include "VulkanSwapChain.h"

class VulkanDepthImage: public VulkanImage {
public:
	VulkanDepthImage();
	VulkanDepthImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSwapChain swapChain, VkFormat format);
	
	static bool hasStencilComponent(VkFormat format);
	static VkFormat findDepthFormat(VulkanPhysicalDevice physicalDevice);
	static VkFormat findSupportedFormat(VulkanPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};
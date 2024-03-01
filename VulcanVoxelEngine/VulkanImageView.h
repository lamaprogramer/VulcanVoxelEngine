#pragma once
#include "VulkanImage.h"

class VulkanImageView {
public:
	VkImageView textureImageView;

	VulkanImageView();
	VulkanImageView(VulkanLogicalDevice device, VkImage image, uint32_t layerCount, VkImageViewType type, VkFormat format, VkImageAspectFlags aspectFlags);

	void destroy(VulkanLogicalDevice device);
};
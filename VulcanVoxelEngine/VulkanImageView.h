#pragma once
#include "VulkanImage.h"

class VulkanImageView {
public:
	VkImageView textureImageView;

	VulkanImageView();
	VulkanImageView(VulkanLogicalDevice device, VulkanImage image, uint32_t layerCount, VkImageViewType type, VkFormat format);
};
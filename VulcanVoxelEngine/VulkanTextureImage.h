#pragma once
#include "VulkanImage.h"
#include "VulkanCommandBufferUtil.h"

class VulkanTextureImage: public VulkanImage {
public:
	VulkanTextureImage();
	VulkanTextureImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, ImageData imageData);

private:
	void transitionImageLayout(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};
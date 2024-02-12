#pragma once
#include "VulkanImageUtil.h"

class VulkanTextureImage {
public:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VulkanTextureImage();
	VulkanTextureImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, const char* path);

private:
	void transitionImageLayout(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};
#pragma once
#include "VulkanBuffer.h"
#include "VulkanCommandBufferUtil.h"
#include <stb_image.h>"

struct ImageData {
	int texWidth;
	int texHeight; 
	int texChannels;
	stbi_uc* pixels;
};

class VulkanImage {
public:
	VkImage image;
	VkDeviceMemory imageMemory;

	VulkanImage();
	VulkanImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, uint32_t width, uint32_t height, uint32_t arrayLayers, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkMemoryPropertyFlags properties);

protected:
	void transitionImageLayout(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkImage image, VkFormat format, uint32_t layerCount, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t layerCount, uint32_t width, uint32_t height);
};
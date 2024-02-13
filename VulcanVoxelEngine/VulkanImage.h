#pragma once
#include "VulkanBuffer.h"
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
	VulkanImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
};
#pragma once
#include "VulkanBufferUtil.h"

class VulkanImageUtil {
public:
	static void createImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
};
#pragma once
#include "VulkanCommandPool.h"
#include "VulkanCommandBufferUtil.h"
#include "Vertex.h"

class VulkanBufferUtil {
public:
	static void createBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static uint32_t findMemoryType(VulkanPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void copyBuffer(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
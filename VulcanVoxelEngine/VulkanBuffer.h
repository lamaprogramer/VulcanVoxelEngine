#pragma once
#include "VulkanCommandPool.h"
#include "Vertex.h"

class VulkanBuffer {
public:
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	VkDescriptorBufferInfo bufferInfo;
	void* bufferMapped;

	VkDeviceSize bufferSize;
	VkDeviceSize bufferOffset = 0;

	VulkanBuffer();
	VulkanBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	void destroy(VulkanLogicalDevice device);
	VkDescriptorBufferInfo createDescriptorBufferInfo(size_t range);
	void updateBufferWithStaging(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, void* data, VkDeviceSize dataSize, VkDeviceSize offset);
	void updateBuffer(VulkanLogicalDevice device, void* bufferData, VkDeviceSize dataSize, VkDeviceSize offset);
	
	static uint32_t findMemoryType(VulkanPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void copyBuffer(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize offset, VkDeviceSize size);
};
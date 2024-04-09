#pragma once
#include "VulkanBuffer.h"
#include "Vertex.h"

class VulkanVertexBuffer: public VulkanBuffer {
public:
	VulkanVertexBuffer();
	VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize);
	VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize, VkMemoryPropertyFlags properties);
};
#pragma once
#include "VulkanBuffer.h"
#include "Vertex.h"

class VulkanVertexBuffer: public VulkanBuffer {
public:
	VkDeviceSize bufferOffset = 0;

	VulkanVertexBuffer();
	VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize);

	void updateVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<Vertex> vertices, VkDeviceSize* start, VkDeviceSize* end);
};
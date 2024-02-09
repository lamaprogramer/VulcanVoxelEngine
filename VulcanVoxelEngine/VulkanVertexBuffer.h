#pragma once
#include "VulkanBufferUtil.h"
#include "Vertex.h"

class VulkanVertexBuffer {
public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VulkanVertexBuffer();
	VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<Vertex> vertices);	
};
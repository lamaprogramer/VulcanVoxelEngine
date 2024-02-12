#pragma once
#include "VulkanDescriptorSetLayout.h"
#include "VulkanBufferUtil.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Matrices.h"

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanUniformBuffer {
public:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorBufferInfo uniformBufferInfo;

	VkDeviceSize uniformSize = 0;

	VulkanUniformBuffer();
	VulkanUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, int maxFramesInFlight);

	void createUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize bufferSize, int maxFramesInFlight, std::vector<VkBuffer> &buffers, std::vector<VkDeviceMemory> &buffersMemory, std::vector<void*> &buffersMapped, VkDescriptorBufferInfo &bufferInfo);
	void updateUniformBuffer(uint32_t currentImage, UniformBufferObject ubo);

	VkDescriptorBufferInfo createDescriptorBufferInfo(VkBuffer buffer, size_t range);

	VkMappedMemoryRange mappedMemoryRange();
	void* alignedAlloc(size_t size, size_t alignment);
};
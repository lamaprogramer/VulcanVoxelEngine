#pragma once
#include "VulkanDescriptorSetLayout.h"
#include "VulkanBufferUtil.h"

#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

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

	VulkanUniformBuffer();
	VulkanUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, int maxFramesInFlight);

	void updateUniformBuffer(uint32_t currentImage, UniformBufferObject ubo);
};
#pragma once
#include "VulkanDescriptorSetLayout.h"
#include "VulkanVertexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Matrices.h"

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanUniformBuffer: public VulkanBuffer {
public:

	VulkanUniformBuffer();
	VulkanUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device);

	void updateUniformBuffer(UniformBufferObject ubo);
};
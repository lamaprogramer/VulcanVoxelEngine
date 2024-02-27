#pragma once
#include "VulkanDescriptorSetLayout.h"
#include "VulkanVertexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Matrices.h"


class VulkanShaderStorageBuffer : public VulkanBuffer {
public:

	VulkanShaderStorageBuffer();
	VulkanShaderStorageBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize size);

	void updateUniformBuffer(const void* ubo);
};
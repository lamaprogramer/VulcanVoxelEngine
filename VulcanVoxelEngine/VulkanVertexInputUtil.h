#pragma once
#include <vulkan/vulkan_core.h>

class VulkanVertexInputUtil {
public:
	static VkVertexInputAttributeDescription createVertexInputAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
};
#include "VulkanVertexInputUtil.h"

VkVertexInputAttributeDescription VulkanVertexInputUtil::createVertexInputAttributeDescription(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset) {
    VkVertexInputAttributeDescription attributeDescriptions{};

    attributeDescriptions.binding = binding;
    attributeDescriptions.location = location;
    attributeDescriptions.format = format;
    attributeDescriptions.offset = offset;

    return attributeDescriptions;
}
#include "VulkanIndexBuffer.h"

VulkanIndexBuffer::VulkanIndexBuffer() {}

VulkanIndexBuffer::VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize) :
    VulkanBuffer{
        physicalDevice,
        device,
        bufferSize,
        //sizeof(indices[0])* indices.size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {}

VulkanIndexBuffer::VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize, VkMemoryPropertyFlags properties) :
    VulkanBuffer{
        physicalDevice,
        device,
        bufferSize,
        //sizeof(indices[0])* indices.size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        properties
} {}
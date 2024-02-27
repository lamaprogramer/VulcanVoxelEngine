#include "VulkanInstanceBuffer.h"

VulkanInstanceBuffer::VulkanInstanceBuffer() {}
VulkanInstanceBuffer::VulkanInstanceBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VkDeviceSize bufferSize):
    VulkanBuffer{
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {}
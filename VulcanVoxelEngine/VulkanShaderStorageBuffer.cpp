#include "VulkanShaderStorageBuffer.h"


VulkanShaderStorageBuffer::VulkanShaderStorageBuffer() {}
VulkanShaderStorageBuffer::VulkanShaderStorageBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize size) :
    VulkanBuffer{
        physicalDevice,
        device,
        size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {
    //vkMapMemory(device.device, bufferMemory, 0, bufferSize, 0, &bufferMapped);
}

void VulkanShaderStorageBuffer::updateUniformBuffer(const void* ubo) {
    memcpy(bufferMapped, &ubo, sizeof(ubo));
}
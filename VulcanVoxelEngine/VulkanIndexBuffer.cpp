#include "VulkanIndexBuffer.h"

VulkanIndexBuffer::VulkanIndexBuffer() {}
VulkanIndexBuffer::VulkanIndexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<uint16_t> indices) :
    VulkanBuffer{
        physicalDevice,
        device,
        sizeof(indices[0])* indices.size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,

} {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VulkanBuffer stagingBuffer = VulkanBuffer(
        physicalDevice,
        device,
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory(device.device, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.device, stagingBuffer.bufferMemory);

    copyBuffer(device, commandPool, stagingBuffer.buffer, buffer, bufferSize);

    vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.device, stagingBuffer.bufferMemory, nullptr);
}
#include "VulkanVertexBuffer.h"

VulkanVertexBuffer::VulkanVertexBuffer() {}
VulkanVertexBuffer::VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize): 
    VulkanBuffer{
        physicalDevice, 
        device, 
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT 
    } {
    bufferSize = bufferSize;
}

void VulkanVertexBuffer::updateVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<Vertex> vertices, VkDeviceSize* start, VkDeviceSize* end) {
    VkDeviceSize verticesSize = sizeof(vertices[0]) * vertices.size();
    
    VulkanBuffer stagingBuffer = VulkanBuffer(
        physicalDevice,
        device,
        verticesSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );


    void* data;
    vkMapMemory(device.device, stagingBuffer.bufferMemory, 0, verticesSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)verticesSize);
    vkUnmapMemory(device.device, stagingBuffer.bufferMemory);

    copyBuffer(device, commandPool, stagingBuffer.buffer, buffer, verticesSize);
    vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.device, stagingBuffer.bufferMemory, nullptr);

    *start = bufferOffset;
    *end = bufferOffset + verticesSize;

    bufferOffset += verticesSize;
}
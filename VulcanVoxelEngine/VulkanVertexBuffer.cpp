#include "VulkanVertexBuffer.h"

VulkanVertexBuffer::VulkanVertexBuffer() {}
VulkanVertexBuffer::VulkanVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize) {
    bufferSize = bufferSize;

    VulkanBufferUtil::createBuffer(
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer,
        vertexBufferMemory
    );
}

void VulkanVertexBuffer::updateVertexBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::vector<Vertex> vertices, VkDeviceSize* start, VkDeviceSize* end) {
    VkDeviceSize verticesSize = sizeof(vertices[0]) * vertices.size();
    
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferUtil::createBuffer(
        physicalDevice,
        device,
        verticesSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );


    void* data;
    vkMapMemory(device.device, stagingBufferMemory, 0, verticesSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)verticesSize);
    vkUnmapMemory(device.device, stagingBufferMemory);

    VulkanBufferUtil::copyBuffer(device, commandPool, stagingBuffer, vertexBuffer, verticesSize);
    vkDestroyBuffer(device.device, stagingBuffer, nullptr);
    vkFreeMemory(device.device, stagingBufferMemory, nullptr);

    *start = bufferOffset;
    *end = bufferOffset + verticesSize;

    bufferOffset += verticesSize;
}
#include "VulkanUniformBuffer.h"


VulkanUniformBuffer::VulkanUniformBuffer() {}
VulkanUniformBuffer::VulkanUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device) :
    VulkanBuffer{
        physicalDevice, 
        device,
        sizeof(UniformBufferObject),
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
} {
    //vkMapMemory(device.device, bufferMemory, 0, bufferSize, 0, &bufferMapped);
}

void VulkanUniformBuffer::updateUniformBuffer(UniformBufferObject ubo) {
    ubo.proj[1][1] *= -1;
    memcpy(bufferMapped, &ubo, sizeof(ubo));
}
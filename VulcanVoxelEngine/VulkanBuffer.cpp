#include "VulkanBuffer.h"
#include "VulkanCommandBufferUtil.h"

VulkanBuffer::VulkanBuffer() {}
VulkanBuffer::VulkanBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    bufferSize = size;
    
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    bufferInfo = createDescriptorBufferInfo(VK_WHOLE_SIZE);
    vkBindBufferMemory(device.device, buffer, bufferMemory, 0);

}

VkDescriptorBufferInfo VulkanBuffer::createDescriptorBufferInfo(size_t range = VK_WHOLE_SIZE) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = range;
    return bufferInfo;
}

uint32_t VulkanBuffer::findMemoryType(VulkanPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanBuffer::copyBuffer(VulkanLogicalDevice device, VulkanCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = VulkanCommandBufferUtil::beginSingleTimeCommands(device, commandPool);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    VulkanCommandBufferUtil::endSingleTimeCommands(device, commandBuffer, commandPool);
}
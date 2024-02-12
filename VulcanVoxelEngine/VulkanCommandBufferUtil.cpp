#include "VulkanCommandBufferUtil.h"

VkCommandBuffer VulkanCommandBufferUtil::beginSingleTimeCommands(VulkanLogicalDevice device, VulkanCommandPool commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = VulkanCommandBufferUtil::beginCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    return commandBuffer;
}

void VulkanCommandBufferUtil::endSingleTimeCommands(VulkanLogicalDevice device, VkCommandBuffer commandBuffer, VulkanCommandPool commandPool) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.graphicsQueue);

    vkFreeCommandBuffers(device.device, commandPool.commandPool, 1, &commandBuffer);
}

VkCommandBufferBeginInfo VulkanCommandBufferUtil::beginCommandBuffer(VkCommandBuffer commandBuffer, int flags) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    return beginInfo;
}
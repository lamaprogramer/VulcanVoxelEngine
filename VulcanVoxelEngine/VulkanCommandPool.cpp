#include "VulkanCommandPool.h"

VulkanCommandPool::VulkanCommandPool() {}

VulkanCommandPool::VulkanCommandPool(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSurface surface) {
    QueueFamilyIndices queueFamilyIndices = VulkanPhysicalDevice::findQueueFamilies(physicalDevice.get(), surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}
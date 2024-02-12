#pragma once
#include "VulkanCommandPool.h"

class VulkanCommandBufferUtil {
public:
    static VkCommandBuffer beginSingleTimeCommands(VulkanLogicalDevice device, VulkanCommandPool commandPool);
    static void endSingleTimeCommands(VulkanLogicalDevice device, VkCommandBuffer commandBuffer, VulkanCommandPool commandPool);
    static VkCommandBufferBeginInfo beginCommandBuffer(VkCommandBuffer commandBuffer, int flags);
};
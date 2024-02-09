#include "VulkanFrameBuffers.h"

VulkanFrameBuffers::VulkanFrameBuffers() {}

VulkanFrameBuffers::VulkanFrameBuffers(VulkanLogicalDevice device, VulkanSwapChain swapChain, VulkanImageViews swapChainImageViews, VulkanRenderPass renderPass) {
    swapChainFramebuffers.resize(swapChainImageViews.swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapChainImageViews.swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device.device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
#include "VulkanDepthImage.h"

VulkanDepthImage::VulkanDepthImage() {}
VulkanDepthImage::VulkanDepthImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSwapChain swapChain, VkFormat format): VulkanImage{
        physicalDevice,
        device,
        swapChain.swapChainExtent.width,
        swapChain.swapChainExtent.height,
        1,
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        0,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {}

bool VulkanDepthImage::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat VulkanDepthImage::findDepthFormat(VulkanPhysicalDevice physicalDevice) {
    return findSupportedFormat(
        physicalDevice,
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat VulkanDepthImage::findSupportedFormat(VulkanPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice.get(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}
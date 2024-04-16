#include "VulkanImageView.h"

VulkanImageView::VulkanImageView() {}
VulkanImageView::VulkanImageView(VulkanLogicalDevice device, VkImage image, uint32_t layerCount, VkImageViewType type,  VkFormat format, VkImageAspectFlags aspectFlags) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = type;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = layerCount;

	if (vkCreateImageView(device.get(), &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}
void VulkanImageView::destroy(VulkanLogicalDevice device) {
	vkDestroyImageView(device.get(), textureImageView, nullptr);
}
#pragma once
#include "VulkanImage.h"

class VulkanTextureImage: public VulkanImage {
public:
	VulkanTextureImage();
	VulkanTextureImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, ImageData imageData);
};
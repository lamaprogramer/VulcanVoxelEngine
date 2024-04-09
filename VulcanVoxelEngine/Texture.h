#pragma once
#include "VulkanImageView.h"
#include "VulkanImageSampler.h"

struct Texture {
	VulkanImage image;
	VulkanImageView imageView;
	VulkanImageSampler sampler;
	VkDescriptorSet descriptor;
};
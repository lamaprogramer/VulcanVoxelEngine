#pragma once
#include "VulkanImage.h"

struct CubeMapData {
	int texWidth{0};
	int texHeight{0};
	int texChannels{0};
	stbi_uc* pixels[6];
};

class VulkanCubeMapImage : public VulkanImage {
public:
	VulkanCubeMapImage();
	VulkanCubeMapImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, CubeMapData cubeMapData);
};
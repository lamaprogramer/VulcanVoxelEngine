#pragma once
#include "VulkanImage.h"

struct CubeMapData {
	int texWidth{0};
	int texHeight{0};
	int texChannels{0};
	stbi_uc* pixels[6];
};

class CubeMap: public VulkanImage {
public:
	CubeMap();
	CubeMap(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, CubeMapData cubeMapData);
};
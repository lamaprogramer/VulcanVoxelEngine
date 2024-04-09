#pragma once

#include <string>
#include <map>
#include "VulkanImageView.h"
#include "VulkanCubeMapImage.h"
#include "VulkanTextureImage.h"
#include "Texture.h"

class TextureManager {
public:
	std::map<std::string, Texture> textures;

	TextureManager();

	void loadTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string texturePath);
	void loadCubeTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string texturePath);

	std::string getFileName(std::string path);
	Texture getTexture(std::string name);
	void destroy(VulkanLogicalDevice device);
};
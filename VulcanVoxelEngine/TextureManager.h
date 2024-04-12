#pragma once

#include <string>
#include <map>
#include "VulkanImageView.h"
#include "VulkanCubeMapImage.h"
#include "VulkanTextureImage.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetLayout.h"
#include "Texture.h"

class TextureManager {
public:

	TextureManager();

	void loadTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout descriptorSetLayout, std::string texturePath);
	void loadCubeTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout descriptorSetLayout, std::string texturePath);

	std::string getFileName(std::string path);
	Texture getTexture(std::string name);
	void destroy(VulkanLogicalDevice device);

private:
	std::map<std::string, Texture> textures;

	void allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, VkDescriptorSet& descriptorSets);

};
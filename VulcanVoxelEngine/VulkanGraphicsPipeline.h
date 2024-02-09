#pragma once

#include "VulkanRenderPass.h"
#include "VulkanDescriptorSetLayout.h"
#include <fstream>

class VulkanGraphicsPipeline {
public:
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VulkanGraphicsPipeline();

	VulkanGraphicsPipeline(VulkanLogicalDevice device, VulkanRenderPass renderPass, VulkanDescriptorSetLayout descriptorSetLayout);
private:
	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};
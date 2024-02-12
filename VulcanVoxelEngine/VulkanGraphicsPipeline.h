#pragma once

#include "VulkanRenderPass.h"
#include "VulkanDescriptorSetLayout.h"
#include <fstream>
#include "BasicObject.h"
#include "Vertex.h"

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
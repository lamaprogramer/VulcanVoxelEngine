#pragma once

#include "VulkanRenderPass.h"
#include "VulkanDescriptorSetLayout.h"
#include <fstream>
#include "BasicObject.h"
#include "Vertex.h"
#include "Instance.h"
#include "VulkanVertexInputUtil.h"

class VulkanGraphicsPipeline {
public:
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VulkanGraphicsPipeline();

	static VulkanGraphicsPipeline create();

	VulkanGraphicsPipeline& createShader(VulkanLogicalDevice device, std::string stageName, std::string path, VkShaderStageFlagBits stage);
	VulkanGraphicsPipeline& createInputAssembly(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
	VulkanGraphicsPipeline& createRasterizer(VkPolygonMode polygonMode);
	VulkanGraphicsPipeline& createMultisampler();
	VulkanGraphicsPipeline& createDepthStencil();
	VulkanGraphicsPipeline& createColorBlender();
	VulkanGraphicsPipeline& createDynamicStates();

	VulkanGraphicsPipeline& build(VulkanLogicalDevice device, VulkanRenderPass renderPass, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
private:

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	std::vector<VkShaderModule> shaderModules;

	std::vector<VkVertexInputBindingDescription> bindingDescription;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineDepthStencilStateCreateInfo depthStencil;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlending;

	std::vector<VkDynamicState> dynamicStates;
	VkPipelineDynamicStateCreateInfo dynamicState;
	VkPipelineViewportStateCreateInfo viewportState;


	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};
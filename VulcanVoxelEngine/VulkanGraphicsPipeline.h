#pragma once

#include "VulkanRenderPass.h"
#include "VulkanDescriptorSetLayout.h"
#include <fstream>
#include "BasicObject.h"
#include "Vertex.h"
#include "Instance.h"
#include "VulkanVertexInputUtil.h"

struct VulkanGraphicsPipeline {
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};

class VulkanGraphicsPipelineBuilder {
public:

	static VulkanGraphicsPipelineBuilder create();
	VulkanGraphicsPipelineBuilder& createShader(VulkanLogicalDevice device, const char* stageName, std::string path, VkShaderStageFlagBits stage);
	VulkanGraphicsPipelineBuilder& createInputAssembly(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
	VulkanGraphicsPipelineBuilder& createRasterizer(VkPolygonMode polygonMode);
	VulkanGraphicsPipelineBuilder& createMultisampler();
	VulkanGraphicsPipelineBuilder& createDepthStencil();
	VulkanGraphicsPipelineBuilder& createColorBlender();
	VulkanGraphicsPipelineBuilder& createDynamicStates();
	VulkanGraphicsPipeline build(VulkanLogicalDevice device, VulkanRenderPass renderPass, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

private:

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};
	std::vector<VkShaderModule> shaderModules{};

	std::vector<VkVertexInputBindingDescription> bindingDescription{};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	VkPipelineMultisampleStateCreateInfo multisampling{};
	VkPipelineDepthStencilStateCreateInfo depthStencil{};

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo colorBlending{};

	std::vector<VkDynamicState> dynamicStates{};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	VkPipelineViewportStateCreateInfo viewportState{};

	VulkanGraphicsPipelineBuilder();

	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};
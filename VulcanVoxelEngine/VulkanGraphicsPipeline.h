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


	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};




//I'm trying to create a wrapper class for `VkPipeline` using the builder design pattern, however I found some weird behavior where the structs for the components of the pipeline would be screwed up after the builder method is finished and returns the pointer to the current builder object. 
//
//For example, all of the structs / pipeline components that have the `pNext` field, such as the vertx assembler, will end up with `0xcccccccccccccccc`
//and all of the structs / pipeline components that have the `flags` field end up with values like `3435973836`.
//
//A whole bunch more values are not as they are supposed to be, but those might be the most important ones.
//
//Here are the files for the builder class :
//
//https://github.com/lamaprogramer/VulcanVoxelEngine/blob/master/VulcanVoxelEngine/VulkanGraphicsPipeline.h
//https://github.com/lamaprogramer/VulcanVoxelEngine/blob/master/VulcanVoxelEngine/VulkanGraphicsPipeline.cpp
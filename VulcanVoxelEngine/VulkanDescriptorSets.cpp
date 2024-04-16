#include "VulkanDescriptorSets.h"

VulkanDescriptorSets::VulkanDescriptorSets() {}
VulkanDescriptorSets::VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout globalDescriptorSetLayout, int maxFramesInFlight) {

	std::vector<VkDescriptorSetLayout> layouts(maxFramesInFlight, globalDescriptorSetLayout.descriptorSetLayout);
	allocateDescriptorSets(device, descriptorPool, layouts, globalDescriptorSets, maxFramesInFlight);

	for (size_t i = 0; i < maxFramesInFlight; i++) {
		std::vector<VkWriteDescriptorSet> descriptorWrites = {
			VulkanDescriptorSet::writeUniformBufferDescriptor(globalDescriptorSets[i], 0, uniformBuffers[i].bufferInfo)
		};

		vkUpdateDescriptorSets(device.get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void VulkanDescriptorSets::allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, std::vector<VkDescriptorSet> &descriptorSets, uint32_t descriptorCount) {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.descriptorPool;
	allocInfo.descriptorSetCount = descriptorCount;
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(descriptorCount);
	if (vkAllocateDescriptorSets(device.get(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

VkDescriptorBufferInfo VulkanDescriptorSets::createDescriptorBufferInfo(VkBuffer buffer) {
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);
	return bufferInfo;
}
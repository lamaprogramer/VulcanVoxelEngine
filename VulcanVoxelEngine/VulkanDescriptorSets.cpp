#include "VulkanDescriptorSets.h"

VulkanDescriptorSets::VulkanDescriptorSets() {}
VulkanDescriptorSets::VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout descriptorSetLayout, VulkanImageView textureImageView, VulkanImageSampler textureSampler,  int maxFramesInFlight) {
	std::vector<VkDescriptorSetLayout> layouts(maxFramesInFlight, descriptorSetLayout.descriptorSetLayout);
	allocateDescriptorSets(device, descriptorPool, layouts, descriptorSets, maxFramesInFlight);

	for (size_t i = 0; i < maxFramesInFlight; i++) {
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView.textureImageView;
		imageInfo.sampler = textureSampler.textureSampler;

		std::vector<VkWriteDescriptorSet> descriptorWrites = {
			writeDescriptorSet(descriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, uniformBuffers[i].bufferInfo),
			writeDescriptorSet(descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, imageInfo)
		};

		vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void VulkanDescriptorSets::allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, std::vector<VkDescriptorSet> &descriptorSets, uint32_t descriptorCount) {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.descriptorPool;
	allocInfo.descriptorSetCount = descriptorCount;
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(descriptorCount);
	if (vkAllocateDescriptorSets(device.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
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

VkWriteDescriptorSet VulkanDescriptorSets::writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorBufferInfo info) {
	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = type;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pBufferInfo = &info;
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	return descriptorWrite;
}

VkWriteDescriptorSet VulkanDescriptorSets::writeDescriptorSet(VkDescriptorSet descriptorSet, VkDescriptorType type, uint32_t binding, VkDescriptorImageInfo info) {
	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = type;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pImageInfo = &info;
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	return descriptorWrite;
}
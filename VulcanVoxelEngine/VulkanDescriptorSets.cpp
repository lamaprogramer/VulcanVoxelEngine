#include "VulkanDescriptorSets.h"

VulkanDescriptorSets::VulkanDescriptorSets() {}
VulkanDescriptorSets::VulkanDescriptorSets(VulkanLogicalDevice device, std::vector<VulkanUniformBuffer> uniformBuffers, VulkanDescriptorPool descriptorPool, VulkanDescriptorSetLayout globalDescriptorSetLayout, VulkanDescriptorSetLayout descriptorSetLayout, std::map<std::string, Texture> textureImageViews, int maxFramesInFlight) {
	
	for (auto const& [name, texture] : textureImageViews) {

		std::vector<VkDescriptorSetLayout> layouts(maxFramesInFlight, descriptorSetLayout.descriptorSetLayout);

		allocateDescriptorSets(device, descriptorPool, layouts, descriptorSets[name]);

		
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.imageView.textureImageView;
		imageInfo.sampler = texture.sampler.textureSampler;

		std::vector<VkWriteDescriptorSet> descriptorWrites = {
			//VulkanDescriptorSet::writeUniformBufferDescriptor(descriptorSets[name][i], 0, uniformBuffers[i].bufferInfo),
			VulkanDescriptorSet::writeImageDescriptor(descriptorSets[name], 0, imageInfo),
			//writeDescriptorSet(descriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, uniformBuffers[i].bufferInfo),
			//writeDescriptorSet(descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, imageInfo)
		};

		vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		
	}

	std::vector<VkDescriptorSetLayout> layouts(maxFramesInFlight, globalDescriptorSetLayout.descriptorSetLayout);
	allocateDescriptorSets(device, descriptorPool, layouts, globalDescriptorSets, maxFramesInFlight);

	for (size_t i = 0; i < maxFramesInFlight; i++) {
		std::vector<VkWriteDescriptorSet> descriptorWrites = {
			VulkanDescriptorSet::writeUniformBufferDescriptor(globalDescriptorSets[i], 0, uniformBuffers[i].bufferInfo),
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
void VulkanDescriptorSets::allocateDescriptorSets(VulkanLogicalDevice device, VulkanDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layouts, VkDescriptorSet& descriptorSets) {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(device.device, &allocInfo, &descriptorSets) != VK_SUCCESS) {
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
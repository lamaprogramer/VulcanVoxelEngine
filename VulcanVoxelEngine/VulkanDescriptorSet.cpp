#include "VulkanDescriptorSet.h"

VkWriteDescriptorSet VulkanDescriptorSet::writeImageDescriptor(VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorImageInfo info) {
	
	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pImageInfo = &info;
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	return descriptorWrite;
}

VkWriteDescriptorSet VulkanDescriptorSet::writeUniformBufferDescriptor(VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorBufferInfo info) {
	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pBufferInfo = &info;
	descriptorWrite.pTexelBufferView = nullptr; // Optional
	return descriptorWrite;
}
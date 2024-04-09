#include "VulkanDescriptorPool.h"

VulkanDescriptorPool::VulkanDescriptorPool() {}
VulkanDescriptorPool::VulkanDescriptorPool(VulkanLogicalDevice device, int maxFramesInFlight) {

	std::vector<VkDescriptorPoolSize> poolSizes = {
			creatPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(maxFramesInFlight)),
			creatPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(maxFramesInFlight))
	};

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 1000;
	//poolInfo.maxSets = static_cast<uint32_t>(maxFramesInFlight) * static_cast<uint32_t>(poolSizes.size());

	if (vkCreateDescriptorPool(device.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

VkDescriptorPoolSize VulkanDescriptorPool::creatPoolSize(VkDescriptorType type, uint32_t descriptorCount) {
	VkDescriptorPoolSize poolSize{};
	poolSize.type = type;
	poolSize.descriptorCount = descriptorCount;
	return poolSize;
}
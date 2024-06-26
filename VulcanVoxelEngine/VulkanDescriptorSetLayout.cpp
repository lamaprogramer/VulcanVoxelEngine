#include "VulkanDescriptorSetLayout.h"

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout() {}
VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanLogicalDevice device, std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings) {

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    layoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(device.get(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

VkDescriptorSetLayoutBinding VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags flags, uint32_t binding) {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = binding;
    uboLayoutBinding.descriptorType = type;
    uboLayoutBinding.descriptorCount = 1;

    uboLayoutBinding.stageFlags = flags;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    return uboLayoutBinding;
}
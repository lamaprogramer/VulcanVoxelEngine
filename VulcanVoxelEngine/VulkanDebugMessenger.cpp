#include "VulkanDebugMessenger.h"

VulkanDebugMessenger::VulkanDebugMessenger() {};
    
VulkanDebugMessenger::VulkanDebugMessenger(VulkanInstance instance, bool enableValidationLayers) {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    VulkanInstance::populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance.instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

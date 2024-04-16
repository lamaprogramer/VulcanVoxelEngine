#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanDebugMessenger.h"
#include "VulkanSurface.h"
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class VulkanPhysicalDevice {
public:

	VulkanPhysicalDevice();
	VulkanPhysicalDevice(VulkanInstance instance, VulkanSurface surface, std::vector<const char*> deviceExtensions);

    VkPhysicalDevice& get();

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanSurface surface);

    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VulkanSurface surface);
private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	bool isDeviceSuitable(VkPhysicalDevice device, VulkanSurface surface, std::vector<const char*> deviceExtensions);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions);
};
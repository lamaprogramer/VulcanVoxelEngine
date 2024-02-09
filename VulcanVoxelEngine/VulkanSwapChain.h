#pragma once

#include "VulkanLogicalDevice.h"
#include <algorithm>

class VulkanSwapChain {
public:
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VulkanSwapChain();
	VulkanSwapChain(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanSurface surface, GLFWwindow* window);

private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};
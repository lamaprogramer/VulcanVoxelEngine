#pragma once

#include "VulkanDebugMessenger.h"

class VulkanSurface {
public:
	VkSurfaceKHR surface;

	VulkanSurface();
	VulkanSurface(VulkanInstance instance, GLFWwindow* window);
};
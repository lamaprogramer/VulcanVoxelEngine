#include "VulkanSurface.h"


VulkanSurface::VulkanSurface() {}

VulkanSurface::VulkanSurface(VulkanInstance instance, GLFWwindow* window) {
	if (glfwCreateWindowSurface(instance.instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

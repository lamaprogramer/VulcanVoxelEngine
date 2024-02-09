#pragma once
#include "VulkanLogicalDevice.h"

class VulkanSyncObjects {
public:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	VulkanSyncObjects();
	VulkanSyncObjects(VulkanLogicalDevice device, int maxFramesInFlight);
};
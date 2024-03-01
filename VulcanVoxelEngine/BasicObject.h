#pragma once
#include "VulkanVertexBuffer.h"
#include "Vertex.h"
#include "Instance.h"

class BasicObject {
public:
	Instance instance;
	glm::vec3 worldPosition;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	BasicObject();
	BasicObject(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, glm::mat4 matrix, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
};
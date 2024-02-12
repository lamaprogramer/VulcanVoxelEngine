#pragma once
#include "VulkanVertexBuffer.h"
#include "Vertex.h"

struct ModelMatrixObject{
	glm::mat4 modelMatrix;
};

class BasicObject {
public:
	ModelMatrixObject matrix;
	glm::vec3 worldPosition;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	VkDeviceSize start;
	VkDeviceSize end;

	BasicObject();
	BasicObject(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, glm::mat4 matrix, std::vector<Vertex> vertices, std::vector<uint16_t> indices, VulkanVertexBuffer &vertexBuffer);
};
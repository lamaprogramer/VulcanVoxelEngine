#pragma once
#include "Vertex.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

struct Model {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VulkanVertexBuffer vertexBuffer;
	VulkanIndexBuffer indexBuffer;
};
#pragma once

#include "BasicObject.h"

class CubeObject: public BasicObject {
public:
    static std::vector<Vertex> cubeVertices;
    static std::vector<uint16_t> cubeIndices;

	CubeObject();
	CubeObject(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, glm::mat4 matrix);
};
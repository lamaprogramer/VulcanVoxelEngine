#pragma once
#include "VulkanVertexBuffer.h"
#include "Vertex.h"
#include "Instance.h"
#include "Matrices.h"

class BasicObject {
public:
	Instance instance;
	glm::vec3 worldPosition;
	std::string modelName;
	std::string textureName;

	BasicObject();
	BasicObject(glm::vec3 position, std::string modelName, std::string textureName);

	void updatePosition(glm::vec3 position);
};
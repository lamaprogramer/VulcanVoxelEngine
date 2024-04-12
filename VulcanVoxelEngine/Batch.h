#pragma once
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanInstanceBuffer.h"
#include "VulkanCommandPool.h"
#include "BasicObject.h"

class Batch {
public:
	std::string modelName;
	std::string textureName;
	VkDescriptorSet descriptorSet;
	std::vector<Instance> instanceData;

	Batch();
	Batch(std::string modelName, std::string textureName);

	//void init(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool);
	//void destroy(VulkanLogicalDevice device);

	static std::vector<Batch> createBatches(std::vector<BasicObject> objects);
	static bool similarObjects(BasicObject &firstObject, BasicObject &secondObject);
};
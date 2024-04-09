#pragma once
#include "Batch.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "VulkanDescriptorSets.h"

class BatchManager {
public:
	uint32_t currentBuffer = 0;

	uint32_t currentBatch = 0;
	bool batchesComplete = false;
	std::vector<Batch> batches;
	
	std::vector<VulkanVertexBuffer> vertexBuffer;
	std::vector<VulkanIndexBuffer> indexBuffer;
	std::vector<VulkanInstanceBuffer> instanceBuffer;

	BatchManager();
	BatchManager(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize);

	void update(std::vector<Batch> batches);

	void nextBatch(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, ModelManager manager);

	void finishBatch();

	void destroy(VulkanLogicalDevice device);

	VulkanVertexBuffer& getVertexBuf();
	VulkanIndexBuffer& getIndexBuf();
	VulkanInstanceBuffer& getInstanceBuf();
};
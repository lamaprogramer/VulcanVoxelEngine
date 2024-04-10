#pragma once
#include "Batch.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "VulkanDescriptorSets.h"

class BatchManager {
public:

	BatchManager();
	BatchManager(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize);

	void update(VulkanLogicalDevice device, std::vector<Batch> batches);
	bool hasNext();
	void finishBatch();
	void destroy(VulkanLogicalDevice device);

	Batch getBatch();
	int getInstanceOffset();
	VulkanInstanceBuffer& getInstanceBuf();

private:
	int instanceOffset = 0;
	VulkanInstanceBuffer instanceBuffer;
	std::vector<Batch> batches;

};
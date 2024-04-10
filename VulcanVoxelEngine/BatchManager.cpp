#include "BatchManager.h"

BatchManager::BatchManager() {

}

BatchManager::BatchManager(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize) {
	this->instanceBuffer = VulkanInstanceBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void BatchManager::update(VulkanLogicalDevice device, std::vector<Batch> batches) {
	this->batches = batches;
	this->instanceOffset = 0;
	this->instanceBuffer.bufferOffset = 0;

	for (Batch batch : batches) {
		instanceBuffer.updateBuffer(
			device,
			batch.instanceData.data(),
			sizeof(Instance) * batch.instanceData.size(),
			instanceBuffer.bufferOffset
		);
	}
}

bool BatchManager::hasNext() {
	return !batches.empty();
}

void BatchManager::finishBatch() {
	Batch& batch = batches[0];
	instanceOffset += batch.instanceData.size();
	batches.erase(batches.begin());
}

void BatchManager::destroy(VulkanLogicalDevice device) {
	instanceBuffer.destroy(device);
}

Batch BatchManager::getBatch() {
	return batches[0];
}

int BatchManager::getInstanceOffset() {
	return instanceOffset;
}

VulkanInstanceBuffer& BatchManager::getInstanceBuf() {
	return instanceBuffer;
}
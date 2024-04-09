#include "BatchManager.h"

BatchManager::BatchManager() {

}

BatchManager::BatchManager(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, long bufferSize) {
	this->instanceBuffer = { 
		VulkanInstanceBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		VulkanInstanceBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};
	this->vertexBuffer = { 
		VulkanVertexBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		VulkanVertexBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};
	this->indexBuffer = { 
		VulkanIndexBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		VulkanIndexBuffer(physicalDevice, device, commandPool, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};
}

void BatchManager::update(std::vector<Batch> batches) {
	this->batchesComplete = false;
	this->currentBatch = 0;
	this->batches = batches;

}

void BatchManager::nextBatch(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, ModelManager manager) {

	if (!batchesComplete) {
		Batch& batch = batches[currentBatch];
		Model& model = manager.getModel(batch.modelName);

		for (Instance i : batch.instanceData) {
			//std::cout << batch.textureName << ":" << (i.modelMatrix * glm::vec4(1)).y << "\n";
		}
		//std::cout << batch.textureName << ":" << batch.instanceData.size() << "\n";
		/*indexBuffer[currentBuffer].updateBufferWithStaging(
			physicalDevice,
			device,
			commandPool,
			batch.indices.data(),
			sizeof(batch.indices[0]) * batch.indices.size(),
			0
		);*/

		indexBuffer[currentBuffer].updateBuffer(
			device,
			model.indices.data(),
			sizeof(model.indices[0]) * model.indices.size(),
			0
		);

		vertexBuffer[currentBuffer].updateBuffer(
			device,
			model.vertices.data(),
			sizeof(model.vertices[0]) * model.vertices.size(),
			0
		);

		instanceBuffer[currentBuffer].updateBuffer(
			device,
			batch.instanceData.data(),
			sizeof(Instance) * batch.instanceData.size(),
			0
		);
	}

	if (currentBatch == batches.size() - 1) {
		batchesComplete = true;
	}
}

void BatchManager::finishBatch() {
	currentBatch++;
	//currentBuffer = (currentBuffer + 1) % 1;
}

void BatchManager::destroy(VulkanLogicalDevice device) {
	for (VulkanInstanceBuffer buf : instanceBuffer) {
		buf.destroy(device);
	}
	for (VulkanVertexBuffer buf : vertexBuffer) {
		buf.destroy(device);
	}
	for (VulkanIndexBuffer buf : indexBuffer) {
		buf.destroy(device);
	}
}

VulkanVertexBuffer& BatchManager::getVertexBuf() {
	return vertexBuffer[currentBuffer];
}

VulkanIndexBuffer& BatchManager::getIndexBuf() {
	return indexBuffer[currentBuffer];
}

VulkanInstanceBuffer& BatchManager::getInstanceBuf() {
	return instanceBuffer[currentBuffer];
}
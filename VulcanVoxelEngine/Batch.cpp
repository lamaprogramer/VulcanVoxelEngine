#include "Batch.h"

Batch::Batch() {

}

Batch::Batch(std::string modelName, std::string textureName) {
	this->modelName = modelName;
	this->textureName = textureName;
}

//void Batch::destroy(VulkanLogicalDevice device) {
//	indexBuffer.destroy(device);
//	vertexBuffer.destroy(device);
//	instanceBuffer.destroy(device);
//}

std::vector<Batch> Batch::createBatches(std::vector<BasicObject> objects, std::vector<Instance> instanceData) {
	std::vector<Batch> batches{};
	std::vector<int> uniqueObjectIndexes{};


	for (int i = 0; i < objects.size(); i++) {
		BasicObject currentObject = objects[i];
		Instance currentInstance = instanceData[i];

		bool matchesAny = false;
		if (!batches.empty()) {
			for (int j = 0; j < uniqueObjectIndexes.size(); j++) {
				int index = uniqueObjectIndexes[j];
				BasicObject &uniqueObject = objects[index]; // fisrt occurence of unique object

				if (similarObjects(uniqueObject, currentObject)) {
					matchesAny = true;
					Batch &currentBatch = batches[j];
					currentBatch.instanceData.push_back(currentInstance);
					//std::cout << i << "\n";
				}
			}
		}

		if (batches.empty() || !matchesAny) {
			uniqueObjectIndexes.push_back(i);
			Batch batch = Batch(currentObject.modelName, currentObject.textureName);
			batch.instanceData.push_back(currentInstance);
			batches.push_back(batch);
		}
	}

	return batches;
}

bool Batch::similarObjects(BasicObject &firstObject, BasicObject &secondObject) {
	return firstObject.modelName == secondObject.modelName &&
		firstObject.textureName == secondObject.textureName;
}
#include "BasicObject.h"

BasicObject::BasicObject() {

}
BasicObject::BasicObject(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, glm::mat4 modelMatrix, std::vector<Vertex> vertices, std::vector<uint16_t> indices, VulkanVertexBuffer& vertexBuffer) {
	this->matrix = { modelMatrix };
	this->vertices = vertices;
	this->indices = indices;

	vertexBuffer.updateVertexBuffer(physicalDevice, device, commandPool, vertices, &start, &end);
}
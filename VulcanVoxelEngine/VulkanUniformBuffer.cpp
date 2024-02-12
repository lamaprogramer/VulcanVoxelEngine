#include "VulkanUniformBuffer.h"


VulkanUniformBuffer::VulkanUniformBuffer() {}
VulkanUniformBuffer::VulkanUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, int maxFramesInFlight) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    createUniformBuffer(physicalDevice, device, bufferSize, maxFramesInFlight, uniformBuffers, uniformBuffersMemory, uniformBuffersMapped, uniformBufferInfo);
    uniformSize = bufferSize;
}

VkDescriptorBufferInfo VulkanUniformBuffer::createDescriptorBufferInfo(VkBuffer buffer, size_t range = VK_WHOLE_SIZE) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = range;
    return bufferInfo;
}

void VulkanUniformBuffer::createUniformBuffer(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VkDeviceSize bufferSize, int maxFramesInFlight, std::vector<VkBuffer> &buffers, std::vector<VkDeviceMemory> &buffersMemory, std::vector<void*> &buffersMapped, VkDescriptorBufferInfo& bufferInfo) {

    buffers.resize(maxFramesInFlight);
    buffersMemory.resize(maxFramesInFlight);
    buffersMapped.resize(maxFramesInFlight);

    for (size_t i = 0; i < maxFramesInFlight; i++) {
        VulkanBufferUtil::createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffers[i], buffersMemory[i]);
        bufferInfo = createDescriptorBufferInfo(buffers[i]);
        vkMapMemory(device.device, buffersMemory[i], 0, bufferSize, 0, &buffersMapped[i]);
    }
}

void VulkanUniformBuffer::updateUniformBuffer(uint32_t currentImage, UniformBufferObject ubo) {
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

inline VkMappedMemoryRange VulkanUniformBuffer::mappedMemoryRange() {
    VkMappedMemoryRange mappedMemoryRange{};
    mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    return mappedMemoryRange;
}

void* VulkanUniformBuffer::alignedAlloc(size_t size, size_t alignment) {
    void* data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else
    int res = posix_memalign(&data, alignment, size);
    if (res != 0)
        data = nullptr;
#endif
    return data;
}
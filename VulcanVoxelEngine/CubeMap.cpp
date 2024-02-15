#include "CubeMap.h"
#define STB_IMAGE_IMPLEMENTATION

CubeMap::CubeMap() {}
CubeMap::CubeMap(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, CubeMapData cubeMapData) : VulkanImage{
        physicalDevice,
        device,
        cubeMapData.texWidth,
        cubeMapData.texHeight,
        6,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {
    const VkDeviceSize imageSize = (cubeMapData.texWidth) * (cubeMapData.texHeight) * 4 * 6;
    const VkDeviceSize layerSize = imageSize / 6;

    // Create staging buffer
    VulkanBuffer stagingBuffer = VulkanBuffer(
        physicalDevice,
        device,
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory(device.device, stagingBuffer.bufferMemory, 0, imageSize, 0, &data);
    for (int i = 0; i < 6; ++i) {
        memcpy(static_cast<char*>(data) + (layerSize * i), cubeMapData.pixels[i], layerSize);
    }
    vkUnmapMemory(device.device, stagingBuffer.bufferMemory);
    for (int i = 0; i < 6; ++i) {
        stbi_image_free(cubeMapData.pixels[i]); // Free the image memory
    }

    transitionImageLayout(
        device,
        commandPool,
        image,
        VK_FORMAT_R8G8B8A8_SRGB,
        6,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    copyBufferToImage(device, commandPool, stagingBuffer.buffer, image, 6, static_cast<uint32_t>(cubeMapData.texWidth), static_cast<uint32_t>(cubeMapData.texHeight));
    transitionImageLayout(
        device,
        commandPool,
        image,
        VK_FORMAT_R8G8B8A8_SRGB,
        6,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.device, stagingBuffer.bufferMemory, nullptr);
}
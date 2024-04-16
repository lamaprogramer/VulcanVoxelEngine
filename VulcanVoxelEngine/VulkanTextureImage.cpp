#include "VulkanTextureImage.h"

VulkanTextureImage::VulkanTextureImage() {}
VulkanTextureImage::VulkanTextureImage(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, ImageData imageData): VulkanImage{
        physicalDevice,
        device,
        imageData.texWidth,
        imageData.texHeight,
        1,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
} {
    VkDeviceSize imageSize = imageData.texWidth * imageData.texHeight * 4;

    // Create staging buffer
    VulkanBuffer stagingBuffer = VulkanBuffer(
        physicalDevice, 
        device, 
        imageSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory(device.get(), stagingBuffer.bufferMemory, 0, imageSize, 0, &data);
    memcpy(data, imageData.pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device.get(), stagingBuffer.bufferMemory);
    stbi_image_free(imageData.pixels); // Free the image memory

    transitionImageLayout(
        device,
        commandPool,
        image,
        VK_FORMAT_R8G8B8A8_SRGB,
        1,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    copyBufferToImage(device, commandPool, stagingBuffer.buffer, image, 1, static_cast<uint32_t>(imageData.texWidth), static_cast<uint32_t>(imageData.texHeight));
    transitionImageLayout(
        device, 
        commandPool, 
        image, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        1,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    vkDestroyBuffer(device.get(), stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.get(), stagingBuffer.bufferMemory, nullptr);
}
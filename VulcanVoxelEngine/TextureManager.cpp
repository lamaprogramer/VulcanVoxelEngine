#include "TextureManager.h"

TextureManager::TextureManager() {

}

void TextureManager::loadTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string texturePath) {
    Texture texture{};

    ImageData imageData{};
    imageData.pixels = stbi_load(texturePath.c_str(), &imageData.texWidth, &imageData.texHeight, &imageData.texChannels, STBI_rgb_alpha);

    texture.image = VulkanTextureImage(physicalDevice, device, commandPool, imageData);
    texture.imageView = VulkanImageView(device, texture.image.image, 1, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
    texture.sampler = VulkanImageSampler(physicalDevice, device);

    std::string fileName = getFileName(texturePath);
    textures[fileName] = texture;
}

void TextureManager::loadCubeTexture(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string texturePath) {
    Texture texture{};
    
    CubeMapData cubeMapData{};

    for (int i = 0; i < 6; i++) {
        cubeMapData.pixels[i] = stbi_load(texturePath.c_str(), &cubeMapData.texWidth, &cubeMapData.texHeight, &cubeMapData.texChannels, STBI_rgb_alpha);
    }

    texture.image = VulkanCubeMapImage(physicalDevice, device, commandPool, cubeMapData);
    texture.imageView = VulkanImageView(device, texture.image.image, 6, VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
    texture.sampler = VulkanImageSampler(physicalDevice, device);

    std::string fileName = getFileName(texturePath);
    textures[fileName] = texture;
}

std::string TextureManager::getFileName(std::string path) {
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);

    std::string::size_type const p(base_filename.find_last_of('.'));
    return base_filename.substr(0, p);
}

void TextureManager::destroy(VulkanLogicalDevice device) {
    for (auto& [name, texture] : textures) {
        texture.image.destroy(device);
        texture.imageView.destroy(device);
        texture.sampler.destroy(device);
    }
}

Texture TextureManager::getTexture(std::string name) {
    return textures[name];
}
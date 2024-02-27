#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <array>


struct Instance {
    glm::mat4 modelMatrix;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(Instance);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescription;
    }
};
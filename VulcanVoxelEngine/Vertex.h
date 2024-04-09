#pragma once
#include <vulkan/vulkan_core.h>
#include <functional>
#include <glm/glm.hpp>
#include <array>


struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    /*bool operator == (const Vertex& c) {
        return pos == c.pos &&
            color == c.color &&
            texCoord == c.texCoord;
    }

    bool operator != (const Vertex& c) {
        return pos != c.pos ||
            color != c.color ||
            texCoord != c.texCoord;
    }*/
};
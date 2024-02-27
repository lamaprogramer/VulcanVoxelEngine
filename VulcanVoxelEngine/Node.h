#pragma once

#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <array>

struct Node {
    glm::mat4 modelMatrix;
};

struct Instances {
    std::vector<Node> nodes;
};
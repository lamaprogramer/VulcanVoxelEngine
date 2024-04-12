#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "Engine.h"


class App : public Engine {
public:
    void loadResources(ModelManager& modelManager, TextureManager& textureManager) {
        modelManager.addModel(physicalDevice, device, commandPool, "cube", CubeObject::cubeVertices, CubeObject::cubeIndices);
        modelManager.addModel(physicalDevice, device, commandPool, "cube2", CubeObject::cubeVertices, CubeObject::cubeIndices);
        modelManager.loadModel(physicalDevice, device, commandPool, "models/viking_room.obj");
        modelManager.loadModel(physicalDevice, device, commandPool, "models/plane.obj");

        textureManager.loadCubeTexture(physicalDevice, device, commandPool, descriptorPool, descriptorSetLayout, "textures/cube.png");
        textureManager.loadCubeTexture(physicalDevice, device, commandPool, descriptorPool, descriptorSetLayout, "textures/viking_room.png");
    }
};

int main() {
    App app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
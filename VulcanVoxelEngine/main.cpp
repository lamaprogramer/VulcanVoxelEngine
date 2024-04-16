#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

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

    void loadObjects(std::vector<BasicObject>& objectList) {
        for (int y = 0; y < 1; y++) {
            for (int z = -25; z < 25; z++) {
                for (int x = -25; x < 25; x++) {
                    CubeObject object = CubeObject(
                        glm::vec3(x, y, z),
                        "cube2",
                        "cube"
                    );

                    objectList.push_back(object);
                }
            }
        }

        CubeObject object = CubeObject(
            glm::vec3(0, 5, 0),
            "cube2",
            "viking_room"
        );

        object.physics = Physics();
        object.physics.value().velocity = glm::vec3(1, 0, 1);
        objectList.push_back(object);
    }

    void update(std::vector<BasicObject>& objectList) {
        for (int i = 0; i < objectList.size(); i++) {
            BasicObject& object = objectList[i];
            if (object.physics) {
                object.updatePosition(
                    object.worldPosition + object.physics.value().update(deltaTime)
                );
            }
        }
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
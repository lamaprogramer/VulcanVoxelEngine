#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "VulkanFrameBuffers.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffers.h"
#include "VulkanSyncObjects.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanInstanceBuffer.h"
#include "VulkanDescriptorSets.h"
#include "VulkanTextureImage.h"
#include "VulkanImageView.h"
#include "VulkanImageSampler.h"
#include "VulkanCubeMapImage.h"

#include "Vertex.h"
#include "Camera.h"
#include "CubeObject.h"
#include "Chunk.h"
#include "Batch.h"
#include "BatchManager.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

std::vector<BasicObject> objectList{};
std::vector<Instance> instanceData{};


std::vector<std::vector<Chunk>> world = std::vector(64, std::vector<Chunk>(64));



#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    float secondsPassed = 0;
    int fps = 0;

    VulkanInstance instance;
    VulkanDebugMessenger debugMessenger;
    VulkanSurface surface;
    VulkanPhysicalDevice physicalDevice;
    VulkanLogicalDevice device;
    VulkanSwapChain swapChain;
    std::vector<VulkanImageView> swapChainImageViews;
    VulkanRenderPass renderPass;

    VulkanDescriptorSetLayout descriptorSetLayout;
    VulkanDescriptorSetLayout globalDescriptorSetLayout;

    VulkanGraphicsPipeline graphicsPipeline;
    VulkanFrameBuffers swapChainFramebuffers;
    VulkanCommandPool commandPool;
    VulkanDepthImage depthImage;
    VulkanImageView depthImageView;
    VulkanTextureImage textureImage;
    VulkanCubeMapImage cubeMap;
    VulkanImageView textureImageView;
    VulkanImageSampler textureSampler;

    ModelManager modelManager;
    TextureManager textureManager;
    BatchManager batchManager;

    std::vector<VulkanUniformBuffer> uniformBuffers;
    VulkanDescriptorPool descriptorPool;
    VulkanDescriptorSets descriptorSets;
    VulkanCommandBuffers commandBuffers;
    VulkanSyncObjects syncObjects;
    Camera camera;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f;

    float lastX = 400, lastY = 300;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create opengl context.
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }

    void initVulkan() {
        instance =              VulkanInstance(enableValidationLayers, validationLayers);
        debugMessenger =        VulkanDebugMessenger(instance, enableValidationLayers);
        surface =               VulkanSurface(instance, window);
        physicalDevice =        VulkanPhysicalDevice(instance, surface, deviceExtensions);
        device =                VulkanLogicalDevice(physicalDevice, surface, deviceExtensions, validationLayers, enableValidationLayers);
        swapChain =             VulkanSwapChain(physicalDevice, device, surface, window);

        swapChainImageViews.resize(swapChain.swapChainImages.size());
        for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
            swapChainImageViews[i] = VulkanImageView(device, swapChain.swapChainImages[i], 1, VK_IMAGE_VIEW_TYPE_2D, swapChain.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }

        renderPass =            VulkanRenderPass(physicalDevice, device, swapChain);

        descriptorSetLayout = VulkanDescriptorSetLayout(device, std::vector{
                //VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
                VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0)
            }
        );

        globalDescriptorSetLayout = VulkanDescriptorSetLayout(device, std::vector{
                VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
            }
        );

        graphicsPipeline = VulkanGraphicsPipeline(device, renderPass, std::vector{
                globalDescriptorSetLayout.descriptorSetLayout,
                descriptorSetLayout.descriptorSetLayout
            }
        );
        
        // Depth checking
        VkFormat depthFormat =  VulkanDepthImage::findDepthFormat(physicalDevice);
        depthImage =            VulkanDepthImage(physicalDevice, device, swapChain, depthFormat);
        depthImageView =        VulkanImageView(device, depthImage.image, 1, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        
        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, depthImageView, renderPass);
        commandPool =           VulkanCommandPool(physicalDevice, device, surface);

        modelManager = ModelManager();
        modelManager.addModel(physicalDevice, device, commandPool, "cube", CubeObject::cubeVertices, CubeObject::cubeIndices);
        modelManager.addModel(physicalDevice, device, commandPool, "cube2", CubeObject::cubeVertices, CubeObject::cubeIndices);
        modelManager.loadModel(physicalDevice, device, commandPool, "models/viking_room.obj");

        textureManager = TextureManager();
        textureManager.loadCubeTexture(physicalDevice, device, commandPool, "textures/cube.png");
        textureManager.loadCubeTexture(physicalDevice, device, commandPool, "textures/viking_room.png");

        batchManager = BatchManager(physicalDevice, device, commandPool, 1048576 * 40);

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VulkanUniformBuffer buffer = VulkanUniformBuffer(physicalDevice, device);
            uniformBuffers[i] = buffer;
        }

        descriptorPool =        VulkanDescriptorPool(device, MAX_FRAMES_IN_FLIGHT);

        descriptorSets =        VulkanDescriptorSets(device, uniformBuffers, descriptorPool, globalDescriptorSetLayout, descriptorSetLayout, textureManager.textures, MAX_FRAMES_IN_FLIGHT);

        commandBuffers =        VulkanCommandBuffers(device, commandPool, MAX_FRAMES_IN_FLIGHT);
        syncObjects =           VulkanSyncObjects(device, MAX_FRAMES_IN_FLIGHT);

        camera = Camera(window, glm::vec3(4, 1, 4), swapChain.swapChainExtent.width / 2, swapChain.swapChainExtent.height / 2);

        //BasicObject vikingRoom = BasicObject(glm::vec3(0, 12, 0), "viking_room", "viking_room");
        //objectList.push_back(vikingRoom);
        //instanceData.push_back(vikingRoom.instance);

        for (int y = 0; y < 10; y++) {
            for (int z = 0; z < 10; z++) {
                for (int x = 0; x < 10; x++) {
                    CubeObject object = CubeObject(
                        glm::vec3(x, y, z),
                        "cube2",
                        y < 5 ? "cube" : "viking_room"
                    );

                    //objectList[z * 100 + x] = object;
                    objectList.push_back(object);
                    instanceData.push_back(object.instance);
                    //instanceData[z * 100 + x] = object.instance;
                }
            }
        }
        CubeObject object = CubeObject(
            glm::vec3(0, 13, 0),
            "viking_room",
            "viking_room"
        );

        ////objectList[z * 100 + x] = object;
        objectList.push_back(object);
        instanceData.push_back(object.instance);
    }


    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(device.device);
    }

    void cleanup() {
        cleanupSwapChain();

        depthImageView.destroy(device);
        depthImage.destroy(device);


        textureManager.destroy(device);
        modelManager.destroy(device);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers[i].destroy(device);
        }
        vkDestroyDescriptorPool(device.device, descriptorPool.descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(device.device, descriptorSetLayout.descriptorSetLayout, nullptr);
        vkDestroyDescriptorSetLayout(device.device, globalDescriptorSetLayout.descriptorSetLayout, nullptr);

        batchManager.destroy(device);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device.device, syncObjects.renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device.device, syncObjects.imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device.device, syncObjects.inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(device.device, commandPool.commandPool, nullptr);
        vkDestroyRenderPass(device.device, renderPass.renderPass, nullptr);

        vkDestroyPipeline(device.device, graphicsPipeline.graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device.device, graphicsPipeline.pipelineLayout, nullptr);

        vkDestroyDevice(device.device, nullptr);

        if (enableValidationLayers) {
            VulkanDebugMessenger::DestroyDebugUtilsMessengerEXT(instance.instance, debugMessenger.debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance.instance, surface.surface, nullptr);
        vkDestroyInstance(instance.instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    // Helper functions.
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void cleanupSwapChain() {
        for (auto framebuffer : swapChainFramebuffers.swapChainFramebuffers) {
            vkDestroyFramebuffer(device.device, framebuffer, nullptr);
        }

        for (auto &imageView : swapChainImageViews) {
            vkDestroyImageView(device.device, imageView.textureImageView, nullptr);
        }

        vkDestroySwapchainKHR(device.device, swapChain.swapChain, nullptr);
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device.device);
        cleanupSwapChain();

        swapChain = VulkanSwapChain(physicalDevice, device, surface, window);

        for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
            swapChainImageViews[i] = VulkanImageView(device, swapChain.swapChainImages[i], 1, VK_IMAGE_VIEW_TYPE_2D, swapChain.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }

        VkFormat depthFormat = VulkanDepthImage::findDepthFormat(physicalDevice);
        depthImage = VulkanDepthImage(physicalDevice, device, swapChain, depthFormat);
        depthImageView = VulkanImageView(device, depthImage.image, 1, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, depthImageView, renderPass);
    }

    void drawFrame() {
        // Calculate FPS
        fps += 1;
        float currentTime = glfwGetTime();
        if (currentTime - secondsPassed > 1) {
            secondsPassed = currentTime;
            std::cout << "FPS: " << fps << "\n";
            fps = 0;
        }

        // calculate delta
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        vkWaitForFences(device.device, 1, &syncObjects.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            framebufferResized = false;
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        // Only reset the fence if we are submitting work
        vkResetFences(device.device, 1, &syncObjects.inFlightFences[currentFrame]);


        vkResetCommandBuffer(commandBuffers.commandBuffers[currentFrame], 0);

        recordCommandBuffer(commandBuffers.commandBuffers[currentFrame], imageIndex);

        camera.update(window, deltaTime);

        /*float speed = 1 * deltaTime;
        for (int i = 0; i < objectList.size(); i++) {
            BasicObject& object = objectList[i];
            object.updatePosition(
                glm::vec3(object.worldPosition.x, object.worldPosition.y + speed, object.worldPosition.z)
            );
            instanceData[i] = object.instance;
        }*/

        UniformBufferObject ubo{};
        ubo.view = camera.getViewMatrix();
        ubo.proj = Matricies::createPerspectiveMatrix(glm::radians(45.0f), swapChain.swapChainExtent.width / (float)swapChain.swapChainExtent.height);

        ubo.proj[1][1] *= -1;
        uniformBuffers[currentFrame].updateBuffer(device, &ubo, sizeof(ubo), 0);


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { syncObjects.imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers.commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = { syncObjects.renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain.swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(device.presentQueue, &presentInfo);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo = VulkanCommandBufferUtil::beginCommandBuffer(commandBuffer, 0);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers.swapChainFramebuffers[imageIndex];

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.swapChainExtent.width);
        viewport.height = static_cast<float>(swapChain.swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChain.swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorSets.globalDescriptorSets[currentFrame], 0, nullptr);
        batchManager.update(device, Batch::createBatches(objectList, instanceData));
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorSets.globalDescriptorSets[currentFrame], 0, nullptr);
        while (batchManager.hasNext()) {

            Batch batch = batchManager.getBatch();
            Model model = modelManager.getModel(batch.modelName);

            VkBuffer vertexBuffers[] = { model.vertexBuffer.buffer};
            VkBuffer instanceBuffers[] = { batchManager.getInstanceBuf().buffer};
            VkDeviceSize offsets[1] = { 0 };

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 1, 1, &descriptorSets.descriptorSets[batch.textureName], 0, nullptr);

            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindVertexBuffers(commandBuffer, 1, 1, instanceBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffer, model.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(model.indices.size()), batch.instanceData.size(), 0, 0, batchManager.getInstanceOffset());

            batchManager.finishBatch();
        }
        
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
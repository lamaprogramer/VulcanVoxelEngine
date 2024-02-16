#include "VulkanFrameBuffers.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffers.h"
#include "VulkanSyncObjects.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSets.h"
#include "VulkanTextureImage.h"
#include "VulkanImageView.h"
#include "VulkanImageSampler.h"
#include "VulkanCubeMapImage.h"

#include "Vertex.h"
#include "Camera.h"
#include "BasicObject.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<Vertex> vertices = {
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // 0
    {{1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 1
    {{1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 2
    {{-1.0f,  1.0f,  1.0f}, { 1.0f, 0.0f, 0.0f }, {0.0f, 0.0f}}, // 3

    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }, // 4
    {{1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // 5
    {{1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} }, // 6
    {{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} } // 7
};

const std::vector<uint16_t> indices = {
        0,1,2, 2,3,0, 1,5,6, 6,2,1, 7,6,5, 5,4,7, 4,0,3, 3,7,4, 4,5,1, 1,0,4, 3,2,6, 6,7,3,
};

const int MAX_OBJECTS = 50;
std::vector<BasicObject> objectList{};



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

    VulkanInstance instance;
    VulkanDebugMessenger debugMessenger;
    VulkanSurface surface;
    VulkanPhysicalDevice physicalDevice;
    VulkanLogicalDevice device;
    VulkanSwapChain swapChain;
    VulkanImageViews swapChainImageViews;
    VulkanRenderPass renderPass;
    VulkanDescriptorSetLayout descriptorSetLayout;
    VulkanGraphicsPipeline graphicsPipeline;
    VulkanFrameBuffers swapChainFramebuffers;
    VulkanCommandPool commandPool;

    VulkanDepthImage depthImage;
    VulkanImageView depthImageView;

    VulkanIndexBuffer indexBuffer;

    VulkanTextureImage textureImage;
    VulkanCubeMapImage cubeMap;
    VulkanImageView textureImageView;
    VulkanImageSampler textureSampler;

    VulkanVertexBuffer vertexBuffer;

    std::vector<VulkanUniformBuffer> uniformBuffers;
    VulkanDescriptorPool descriptorPool;
    VulkanDescriptorSets descriptorSets;

    VulkanCommandBuffers commandBuffers;
    VulkanSyncObjects syncObjects;

    Camera camera;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;

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
        camera =                Camera(window, glm::vec3(0, 0, 4), swapChain.swapChainExtent.width / 2, swapChain.swapChainExtent.height / 2);
        swapChainImageViews =   VulkanImageViews(device, swapChain);
        renderPass =            VulkanRenderPass(physicalDevice, device, swapChain);
        descriptorSetLayout =   VulkanDescriptorSetLayout(device);
        graphicsPipeline =      VulkanGraphicsPipeline(device, renderPass, descriptorSetLayout);
        
        // Depth checking
        VkFormat depthFormat =  VulkanDepthImage::findDepthFormat(physicalDevice);
        depthImage =            VulkanDepthImage(physicalDevice, device, swapChain, depthFormat);
        depthImageView =        VulkanImageView(device, depthImage, 1, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        
        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, depthImageView, renderPass);
        commandPool =           VulkanCommandPool(physicalDevice, device, surface);

        const char* path = "textures/texture.png";
        CubeMapData cubeMapData{};

        for (int i = 0; i < 6; i++) {
            cubeMapData.pixels[i] = stbi_load(path, &cubeMapData.texWidth, &cubeMapData.texHeight, &cubeMapData.texChannels, STBI_rgb_alpha);
        }
        cubeMap =               VulkanCubeMapImage(physicalDevice, device, commandPool, cubeMapData);
        textureImageView =      VulkanImageView(device, cubeMap, 6, VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        textureSampler =        VulkanImageSampler(physicalDevice, device);
        vertexBuffer =          VulkanVertexBuffer(physicalDevice, device, commandPool, 1048576*40);
        indexBuffer =           VulkanIndexBuffer(physicalDevice, device, commandPool, indices);

        for (int i = 0; i < MAX_OBJECTS; i++) {
            objectList.push_back(BasicObject(
                physicalDevice,
                device,
                commandPool,
                Matricies::createModelMatrix(glm::vec3(i*2, 0, -i*2), 1.0f, glm::radians(0.0), glm::vec3(0.0, 0.0, 1.0)),
                vertices,
                indices,
                vertexBuffer
            ));
        }


        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VulkanUniformBuffer buffer = VulkanUniformBuffer(physicalDevice, device);
            uniformBuffers[i] = buffer;
        }

        descriptorPool =        VulkanDescriptorPool(device, MAX_FRAMES_IN_FLIGHT);
        descriptorSets =        VulkanDescriptorSets(device, uniformBuffers, descriptorPool, descriptorSetLayout, textureImageView, textureSampler, MAX_FRAMES_IN_FLIGHT);

        commandBuffers =        VulkanCommandBuffers(device, commandPool, MAX_FRAMES_IN_FLIGHT);
        syncObjects =           VulkanSyncObjects(device, MAX_FRAMES_IN_FLIGHT);
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

        vkDestroyImageView(device.device, depthImageView.textureImageView, nullptr);
        vkDestroyImage(device.device, depthImage.image, nullptr);
        vkFreeMemory(device.device, depthImage.imageMemory, nullptr);

        vkDestroyImageView(device.device, textureImageView.textureImageView, nullptr);
        vkDestroySampler(device.device, textureSampler.textureSampler, nullptr);
        //vkDestroyImage(device.device, textureImage.image, nullptr);
        //vkFreeMemory(device.device, textureImage.imageMemory, nullptr);
        vkDestroyImage(device.device, cubeMap.image, nullptr);
        vkFreeMemory(device.device, cubeMap.imageMemory, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device.device, uniformBuffers[i].buffer, nullptr);
            vkFreeMemory(device.device, uniformBuffers[i].bufferMemory, nullptr);
        }
        vkDestroyDescriptorPool(device.device, descriptorPool.descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device.device, descriptorSetLayout.descriptorSetLayout, nullptr);
        vkDestroyBuffer(device.device, indexBuffer.buffer, nullptr);

        vkFreeMemory(device.device, indexBuffer.bufferMemory, nullptr);

        vkDestroyBuffer(device.device, vertexBuffer.buffer, nullptr);
        vkFreeMemory(device.device, vertexBuffer.bufferMemory, nullptr);
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

        for (auto imageView : swapChainImageViews.swapChainImageViews) {
            vkDestroyImageView(device.device, imageView, nullptr);
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
        swapChainImageViews = VulkanImageViews(device, swapChain);

        VkFormat depthFormat = VulkanDepthImage::findDepthFormat(physicalDevice);
        depthImage = VulkanDepthImage(physicalDevice, device, swapChain, depthFormat);
        depthImageView = VulkanImageView(device, depthImage, 1, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, depthImageView, renderPass);
    }

    void drawFrame() {
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

        camera.update(window);

        UniformBufferObject ubo{};
        ubo.model = Matricies::createModelMatrix(glm::vec3(0, 0, 0), 0.5f, glm::radians(90.0), glm::vec3(0.0, 0.0, 1.0));
        ubo.view = camera.getViewMatrix();
        ubo.proj = Matricies::createPerspectiveMatrix(glm::radians(45.0f), swapChain.swapChainExtent.width / (float)swapChain.swapChainExtent.height);


        uniformBuffers[currentFrame].updateUniformBuffer(ubo);

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

        VkBuffer vertexBuffers[] = { vertexBuffer.buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorSets.descriptorSets[currentFrame], 0, nullptr);
        
        for (int i = 0; i < MAX_OBJECTS; i++) {
            vkCmdPushConstants(commandBuffer, graphicsPipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ModelMatrixObject), (void*)&objectList[i].matrix);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
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
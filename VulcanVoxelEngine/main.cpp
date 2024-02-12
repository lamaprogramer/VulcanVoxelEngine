#include "VulkanFrameBuffers.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffers.h"
#include "VulkanSyncObjects.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSets.h"
#include "VulkanTextureImage.h"

#include "Vertex.h"
#include "Camera.h"
#include "BasicObject.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

using namespace std;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
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
    VulkanIndexBuffer indexBuffer;

    VulkanTextureImage textureImage;
    VulkanVertexBuffer vertexBuffer;

    VulkanUniformBuffer uniformBuffers;
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

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    }

    void initVulkan() {
        instance =              VulkanInstance(enableValidationLayers, validationLayers);
        debugMessenger =        VulkanDebugMessenger(instance, enableValidationLayers);
        surface =               VulkanSurface(instance, window);
        physicalDevice =        VulkanPhysicalDevice(instance, surface, deviceExtensions);
        device =                VulkanLogicalDevice(physicalDevice, surface, deviceExtensions, validationLayers, enableValidationLayers);
        swapChain =             VulkanSwapChain(physicalDevice, device, surface, window);
        swapChainImageViews =   VulkanImageViews(device, swapChain);
        renderPass =            VulkanRenderPass(device, swapChain);
        descriptorSetLayout =   VulkanDescriptorSetLayout(device);
        graphicsPipeline =      VulkanGraphicsPipeline(device, renderPass, descriptorSetLayout);
        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, renderPass);
        commandPool =           VulkanCommandPool(physicalDevice, device, surface);
        textureImage =          VulkanTextureImage(physicalDevice, device, commandPool, "textures/texture.png");

        vertexBuffer =          VulkanVertexBuffer(physicalDevice, device, commandPool, 1048576*40);

        for (int i = 0; i < MAX_OBJECTS; i++) {
            objectList.push_back(BasicObject(
                physicalDevice,
                device,
                commandPool,
                Matricies::createModelMatrix(glm::vec3(i*2, 0, 0), 0.5f, glm::radians(90.0), glm::vec3(0.0, 0.0, 1.0)),
                vertices,
                indices,
                vertexBuffer
            ));
        }

        indexBuffer =           VulkanIndexBuffer(physicalDevice, device, commandPool, indices);


        uniformBuffers =        VulkanUniformBuffer(physicalDevice, device, MAX_FRAMES_IN_FLIGHT);
        descriptorPool =        VulkanDescriptorPool(device, MAX_FRAMES_IN_FLIGHT);
        descriptorSets =        VulkanDescriptorSets(device, uniformBuffers, descriptorPool, descriptorSetLayout, MAX_FRAMES_IN_FLIGHT);


        commandBuffers =        VulkanCommandBuffers(device, commandPool, MAX_FRAMES_IN_FLIGHT);
        syncObjects =           VulkanSyncObjects(device, MAX_FRAMES_IN_FLIGHT);

        camera = Camera(window, glm::vec3(0, 0, 2), swapChain.swapChainExtent.width/2, swapChain.swapChainExtent.height/2);
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

        vkDestroyImage(device.device, textureImage.textureImage, nullptr);
        vkFreeMemory(device.device, textureImage.textureImageMemory, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device.device, uniformBuffers.uniformBuffers[i], nullptr);
            vkFreeMemory(device.device, uniformBuffers.uniformBuffersMemory[i], nullptr);
        }
        vkDestroyDescriptorPool(device.device, descriptorPool.descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device.device, descriptorSetLayout.descriptorSetLayout, nullptr);
        vkDestroyBuffer(device.device, indexBuffer.indexBuffer, nullptr);

        vkFreeMemory(device.device, indexBuffer.indexBufferMemory, nullptr);

        vkDestroyBuffer(device.device, vertexBuffer.vertexBuffer, nullptr);
        vkFreeMemory(device.device, vertexBuffer.vertexBufferMemory, nullptr);
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
        swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, renderPass);
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


        uniformBuffers.updateUniformBuffer(currentFrame, ubo);

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

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

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

        VkBuffer vertexBuffers[] = { vertexBuffer.vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

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
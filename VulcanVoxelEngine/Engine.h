#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "VulkanFrameBuffers.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffers.h"
#include "VulkanSyncObjects.h"

#include "VulkanDescriptorSets.h"
#include "VulkanTextureImage.h"
#include "VulkanCubeMapImage.h"

#include "Camera.h"
#include "CubeObject.h"
#include "EngineKeybinds.h"
#include "BatchManager.h"


const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Engine {
public:
    virtual void run() final;
protected:
    GLFWwindow* window;
    ImGui_ImplVulkanH_Window g_MainWindowData;
    uint32_t minImageCount = 2;

    const uint32_t WIDTH = 1600;
    const uint32_t HEIGHT = 900;
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

    std::vector<VulkanUniformBuffer> uniformBuffers;
    VulkanDescriptorPool descriptorPool;
    VulkanDescriptorSets descriptorSets;
    VulkanCommandBuffers commandBuffers;
    VulkanSyncObjects syncObjects;

    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    Camera camera;
    EngineKeybinds engineKeybinds;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f;

    float lastX = 400, lastY = 300;

    std::vector<BasicObject> objectList{};

    virtual void loadResources(ModelManager& modelManager, TextureManager& textureManager) = 0;
    virtual void loadObjects(std::vector<BasicObject>& objectList) = 0;
    void loadKeybinds(EngineKeybinds& engineKeybinds);
    virtual void update(std::vector<BasicObject>& objectList) = 0;

private:

    ModelManager modelManager;
    TextureManager textureManager;
    BatchManager batchManager;

    void initWindow();

    void initVulkan();

    void initImgui(ImGui_ImplVulkanH_Window* g_MainWindowData);

    void mainLoop();

    void cleanup();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void cleanupSwapChain();

    void recreateSwapChain();

    void drawFrame();

    void updateKeybinds();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }
};
#include "Engine.h"

void Engine::run() {
    initWindow();
    initVulkan();
    initImgui(&g_MainWindowData);
    mainLoop();
    cleanup();
}

void Engine::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create opengl context.
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void Engine::initVulkan() {
    instance = VulkanInstance(enableValidationLayers, validationLayers);
    debugMessenger = VulkanDebugMessenger(instance, enableValidationLayers);
    surface = VulkanSurface(instance, window);
    physicalDevice = VulkanPhysicalDevice(instance, surface, deviceExtensions);
    device = VulkanLogicalDevice(physicalDevice, surface, deviceExtensions, validationLayers, enableValidationLayers);
    swapChain = VulkanSwapChain(physicalDevice, device, surface, window);

    swapChainImageViews.resize(swapChain.swapChainImages.size());
    for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
        swapChainImageViews[i] = VulkanImageView(device, swapChain.swapChainImages[i], 1, VK_IMAGE_VIEW_TYPE_2D, swapChain.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    renderPass = VulkanRenderPass(physicalDevice, device, swapChain);

    descriptorSetLayout = VulkanDescriptorSetLayout(device, std::vector{
            VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0)
        }
    );

    globalDescriptorSetLayout = VulkanDescriptorSetLayout(device, std::vector{
            VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
        }
    );
    graphicsPipeline = VulkanGraphicsPipelineBuilder::create()
        .createShader(device, "main", "shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT)
        .createShader(device, "main", "shaders/cube.spv", VK_SHADER_STAGE_FRAGMENT_BIT)
        .createInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE)
        .createRasterizer(VK_POLYGON_MODE_FILL)
        .createMultisampler()
        .createDepthStencil()
        .createColorBlender()
        .createDynamicStates()
        .build(device, renderPass, std::vector{
                globalDescriptorSetLayout.descriptorSetLayout,
                descriptorSetLayout.descriptorSetLayout
            }
    );

    // Depth checking
    VkFormat depthFormat = VulkanDepthImage::findDepthFormat(physicalDevice);
    depthImage = VulkanDepthImage(physicalDevice, device, swapChain, depthFormat);
    depthImageView = VulkanImageView(device, depthImage.image, 1, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    swapChainFramebuffers = VulkanFrameBuffers(device, swapChain, swapChainImageViews, depthImageView, renderPass);
    commandPool = VulkanCommandPool(physicalDevice, device, surface);
    descriptorPool = VulkanDescriptorPool(device, MAX_FRAMES_IN_FLIGHT*2);

    modelManager = ModelManager();
    textureManager = TextureManager();
    batchManager = BatchManager(physicalDevice, device, commandPool, 1048576 * 40);
    loadResources(modelManager, textureManager);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VulkanUniformBuffer buffer = VulkanUniformBuffer(physicalDevice, device);
        uniformBuffers[i] = buffer;
    }

    descriptorSets = VulkanDescriptorSets(device, uniformBuffers, descriptorPool, globalDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT);
    commandBuffers = VulkanCommandBuffers(device, commandPool, MAX_FRAMES_IN_FLIGHT);
    syncObjects = VulkanSyncObjects(device, MAX_FRAMES_IN_FLIGHT);

    camera = Camera(window, glm::vec3(4, 1, 4), swapChain.swapChainExtent.width / 2, swapChain.swapChainExtent.height / 2);

    loadObjects(objectList);
}

void Engine::initImgui(ImGui_ImplVulkanH_Window* imguiWindow) {
    
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance.get();
    init_info.PhysicalDevice = physicalDevice.get();
    init_info.Device = device.get();
    init_info.Queue = device.getGraphicsQueue();
    init_info.DescriptorPool = descriptorPool.descriptorPool;
    init_info.RenderPass = renderPass.renderPass;
    init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);
}


void Engine::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(device.get());
}

void Engine::cleanup() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    cleanupSwapChain();

    depthImageView.destroy(device);
    depthImage.destroy(device);

    textureManager.destroy(device);
    modelManager.destroy(device);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i].destroy(device);
    }
    vkDestroyDescriptorPool(device.get(), descriptorPool.descriptorPool, nullptr);

    vkDestroyDescriptorSetLayout(device.get(), descriptorSetLayout.descriptorSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(device.get(), globalDescriptorSetLayout.descriptorSetLayout, nullptr);

    batchManager.destroy(device);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device.get(), syncObjects.renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device.get(), syncObjects.imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device.get(), syncObjects.inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device.get(), commandPool.commandPool, nullptr);
    vkDestroyRenderPass(device.get(), renderPass.renderPass, nullptr);

    vkDestroyPipeline(device.get(), graphicsPipeline.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device.get(), graphicsPipeline.pipelineLayout, nullptr);

    vkDestroyDevice(device.get(), nullptr);

    if (enableValidationLayers) {
        VulkanDebugMessenger::DestroyDebugUtilsMessengerEXT(instance.get(), debugMessenger.debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance.get(), surface.surface, nullptr);
    vkDestroyInstance(instance.get(), nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}

// Helper functions.
void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Engine::cleanupSwapChain() {
    for (auto framebuffer : swapChainFramebuffers.swapChainFramebuffers) {
        vkDestroyFramebuffer(device.get(), framebuffer, nullptr);
    }

    for (auto& imageView : swapChainImageViews) {
        vkDestroyImageView(device.get(), imageView.textureImageView, nullptr);
    }

    vkDestroySwapchainKHR(device.get(), swapChain.swapChain, nullptr);
}

void Engine::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    uint32_t queueFamily = VulkanPhysicalDevice::findQueueFamilies(physicalDevice.get(), surface).graphicsFamily.value();
    ImGui_ImplVulkan_SetMinImageCount(minImageCount);
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance.get(), physicalDevice.get(), device.get(), &g_MainWindowData, queueFamily, nullptr, 100, 100, minImageCount);

    vkDeviceWaitIdle(device.get());
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

void Engine::drawFrame() {
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

    vkWaitForFences(device.get(), 1, &syncObjects.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device.get(), swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        framebufferResized = false;
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Only reset the fence if we are submitting work
    vkResetFences(device.get(), 1, &syncObjects.inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers.commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers.commandBuffers[currentFrame], imageIndex);

    camera.update(window, deltaTime);
    update(objectList);

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

    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, syncObjects.inFlightFences[currentFrame]) != VK_SUCCESS) {
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

    vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Engine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
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
    batchManager.update(device, Batch::createBatches(objectList));
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorSets.globalDescriptorSets[currentFrame], 0, nullptr);
    while (batchManager.hasNext()) {

        Batch batch = batchManager.getBatch();
        Model model = modelManager.getModel(batch.modelName);
        Texture texture = textureManager.getTexture(batch.textureName);

        VkBuffer vertexBuffers[] = { model.vertexBuffer.buffer };
        VkBuffer instanceBuffers[] = { batchManager.getInstanceBuf().buffer };
        VkDeviceSize offsets[1] = { 0 };

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 1, 1, &texture.descriptor, 0, nullptr);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, instanceBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, model.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(model.indices.size()), batch.instanceData.size(), 0, 0, batchManager.getInstanceOffset());

        batchManager.finishBatch();
    }

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
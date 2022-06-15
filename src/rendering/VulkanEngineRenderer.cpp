//
// Created by Stanislav Svědiroh on 14.06.2022.
//

#include "VulkanEngineRenderer.h"

VulkanEngineRenderer::VulkanEngineRenderer(VulkanEngineWindow &window, VulkanEngineDevice &device) : window(window),
                                                                                                     engineDevice(
                                                                                                             device) {
    recreateSwapChain();
    createCommandBuffers();
}

VulkanEngineRenderer::~VulkanEngineRenderer() { freeCommandBuffers(); }

VkCommandBuffer VulkanEngineRenderer::beginFrame() {
    assert(!isFrameStarted && "Can't call beginFrame while already in progress!");

    auto result = engineSwapChain->acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
    return commandBuffer;
}

void VulkanEngineRenderer::endFrame() {
    assert(isFrameStarted && "Can't call endFrame while frame is not in progress!");
    auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }

    auto result = engineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
        window.resetWindowsResizedFlag();
        recreateSwapChain();
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void VulkanEngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress!");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = engineSwapChain->getRenderPass();
    renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues = {};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, engineSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

}

void VulkanEngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress!");
    assert(commandBuffer == getCurrentCommandBuffer() &&
           "Can't end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);

}

void VulkanEngineRenderer::createCommandBuffers() {
    commandBuffers.resize(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = engineDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void VulkanEngineRenderer::freeCommandBuffers() {

}

void VulkanEngineRenderer::recreateSwapChain() {
    auto extent = window.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window.getExtent();
        SDL_WaitEvent(nullptr);
    }

    vkDeviceWaitIdle(engineDevice.device());

    if (engineSwapChain == nullptr) {
        engineSwapChain = std::make_unique<VulkanEngineSwapChain>(engineDevice, extent);
    } else {
        std::shared_ptr<VulkanEngineSwapChain> oldSwapChain = std::move(engineSwapChain);
        engineSwapChain = std::make_unique<VulkanEngineSwapChain>(engineDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*engineSwapChain)) {
            throw std::runtime_error("Swap chain image(or depth) format has changed!");
        }
    }
}

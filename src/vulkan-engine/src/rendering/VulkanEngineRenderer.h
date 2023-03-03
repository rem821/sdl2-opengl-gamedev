//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "VulkanEngineWindow.h"
#include "VulkanEngineSwapChain.h"
#include "fmt/include/fmt/core.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

class VulkanEngineRenderer {

public:
    VulkanEngineRenderer(VulkanEngineWindow &window, VulkanEngineDevice &device);
    ~VulkanEngineRenderer();

    VulkanEngineRenderer(const VulkanEngineRenderer &) = delete;
    VulkanEngineRenderer &operator=(const VulkanEngineRenderer &) = delete;

    [[nodiscard]] VkRenderPass getSwapChainRenderPass() const { return engineSwapChain->getRenderPass(); };

    [[nodiscard]] float getAspectRatio() const { return engineSwapChain->extentAspectRatio(); };

    [[nodiscard]] bool isFrameInProgress() const { return isFrameStarted; };

    [[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    VulkanEngineWindow &window;
    VulkanEngineDevice &engineDevice;
    std::unique_ptr<VulkanEngineSwapChain> engineSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex = 0;

    bool isFrameStarted = false;
};
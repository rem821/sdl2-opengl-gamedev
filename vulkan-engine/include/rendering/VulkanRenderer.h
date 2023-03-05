//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <rendering/VulkanDevice.h>
#include <rendering/Window.h>
#include "VulkanSwapChain.h"

namespace VulkanEngine {

    class VulkanRenderer {

    public:
        VulkanRenderer(const Window &window, const VulkanDevice &device);

        ~VulkanRenderer();

        VulkanRenderer(const VulkanRenderer &) = delete;

        VulkanRenderer &operator=(const VulkanRenderer &) = delete;

        [[nodiscard]] VkRenderPass GetSwapChainRenderPass() const { return engineSwapChain_->GetRenderPass(); }

        [[nodiscard]] float GetAspectRatio() const { return engineSwapChain_->GetAspectRatio(); };

        [[nodiscard]] bool IsFrameInProgress() const { return isFrameStarted_; };

        [[nodiscard]] VkCommandBuffer GetCurrentCommandBuffer() const {
            CORE_ASSERT(isFrameStarted_, "Cannot get command buffer when frame not in progress")
            return commandBuffers_[currentFrameIndex_];
        }

        [[nodiscard]] int GetFrameIndex() const {
            CORE_ASSERT(isFrameStarted_, "Cannot get frame index when frame not in progress")
            return currentFrameIndex_;
        }

        VkCommandBuffer BeginFrame();

        void EndFrame();

        void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void CreateCommandBuffers();

        void FreeCommandBuffers();

        void RecreateSwapChain();

        const Window &window_;
        const VulkanDevice &device_;
        std::unique_ptr<VulkanSwapChain> engineSwapChain_;
        std::vector<VkCommandBuffer> commandBuffers_;

        uint32_t currentImageIndex_;
        int currentFrameIndex_ = 0;

        bool isFrameStarted_ = false;
    };
}
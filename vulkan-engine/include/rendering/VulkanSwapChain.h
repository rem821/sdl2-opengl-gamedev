//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <rendering/VulkanDevice.h>

namespace VulkanEngine {
    class VulkanSwapChain {

    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        VulkanSwapChain(const VulkanDevice &engineDevice, VkExtent2D extent);

        VulkanSwapChain(const VulkanDevice &engineDevice, VkExtent2D extent, VulkanSwapChain* previous);

        ~VulkanSwapChain();

        VulkanSwapChain(const VulkanSwapChain &) = delete;

        VulkanSwapChain operator=(const VulkanSwapChain &) = delete;

        VkFramebuffer GetFrameBuffer(uint32_t index) { return swapChainFramebuffers_[index]; };
        VkRenderPass GetRenderPass() { return renderPass_; };
        VkImageView GetImageView(int index) { return swapChainImageViews_[index]; };

        size_t ImageCount() { return swapChainImages_.size(); };
        VkFormat GetSwapChainImageFormat() { return swapChainImageFormat_; };
        VkExtent2D GetSwapChainExtent() { return swapChainExtent_; };
        [[nodiscard]] uint32_t GetWidth() const { return swapChainExtent_.width; };
        [[nodiscard]] uint32_t GetHeight() const { return swapChainExtent_.height; };
        [[nodiscard]] float GetAspectRatio() const {
            return static_cast<float>(swapChainExtent_.width) / static_cast<float>(swapChainExtent_.height);
        };

        VkFormat FindDepthFormat();

        VkResult AcquireNextImage(uint32_t *imageIndex);

        VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);

        [[nodiscard]] bool CompareSwapFormats(const VulkanSwapChain &swapChain) const {
            return swapChain.swapChainDepthFormat_ == swapChainDepthFormat_ &&
                   swapChain.swapChainImageFormat_ == swapChainImageFormat_;
        }
    private:
        void Init();

        void CreateSwapChain();

        void CreateImageViews();

        void CreateDepthResources();

        void CreateRenderPass();

        void CreateFrameBuffers();

        void CreateSyncObjects();

        static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat swapChainImageFormat_{};
        VkFormat swapChainDepthFormat_{};
        VkExtent2D swapChainExtent_{};

        std::vector<VkFramebuffer> swapChainFramebuffers_;
        VkRenderPass renderPass_{};

        std::vector<VkImage> depthImages_;
        std::vector<VkDeviceMemory> depthImageMemory_;
        std::vector<VkImageView> depthImageViews_;
        std::vector<VkImage> swapChainImages_;
        std::vector<VkImageView> swapChainImageViews_;

        const VulkanDevice &device_;
        VkExtent2D windowExtent_;

        VkSwapchainKHR swapChain_{};
        VulkanSwapChain* oldSwapChain_{};

        std::vector<VkSemaphore> imageAvailableSemaphores_;
        std::vector<VkSemaphore> renderFinishedSemaphores_;
        std::vector<VkFence> inFlightFences_;
        std::vector<VkFence> imagesInFlight_;
        size_t currentFrame_ = 0;
    };
}
class VulkanEngineSwapChain {

private:



};

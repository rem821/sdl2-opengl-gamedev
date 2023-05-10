//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#include <platform/vulkan/VulkanRenderer.h>

namespace VulkanEngine {


    VulkanRenderer::VulkanRenderer(const Window &window, const VulkanDevice &device) : window_(window),
                                                                                       device_(device) {
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    VulkanRenderer::~VulkanRenderer() { FreeCommandBuffers(); }

    VkCommandBuffer VulkanRenderer::BeginFrame() {
        CORE_ASSERT(!isFrameStarted_, "Can't call beginFrame while already in progress!")

        auto result = engineSwapChain_->AcquireNextImage(&currentImageIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapChain();
            return nullptr;
        }
        CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image!")

        isFrameStarted_ = true;

        auto commandBuffer = GetCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        CORE_ASSERT(vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS, "Failed to begin recording command buffer!")
        return commandBuffer;
    }

    void VulkanRenderer::EndFrame() {
        CORE_ASSERT(isFrameStarted_, "Can't call endFrame while frame is not in progress!")

        auto commandBuffer = GetCurrentCommandBuffer();

        CORE_ASSERT(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS, "Failed to record command buffer!")

        auto result = engineSwapChain_->SubmitCommandBuffers(&commandBuffer, &currentImageIndex_);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.GetWidth() != engineSwapChain_->GetWidth() ||
            window_.GetHeight() != engineSwapChain_->GetHeight()) {
            RecreateSwapChain();
        }

        isFrameStarted_ = false;
        currentFrameIndex_ = (currentFrameIndex_ + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        CORE_ASSERT(isFrameStarted_, "Can't call beginSwapChainRenderPass if frame is not in progress!")
        CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't begin render pass on command buffer from a different frame")

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = engineSwapChain_->GetRenderPass();
        renderPassInfo.framebuffer = engineSwapChain_->GetFrameBuffer(currentImageIndex_);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = engineSwapChain_->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(engineSwapChain_->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(engineSwapChain_->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, engineSwapChain_->GetSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void VulkanRenderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer) const {
        CORE_ASSERT(isFrameStarted_, "Can't call endSwapChainRenderPass if frame is not in progress!")
        CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't end render pass on command buffer from a different frame")

        vkCmdEndRenderPass(commandBuffer);
    }

    void VulkanRenderer::CreateCommandBuffers() {
        commandBuffers_.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device_.GetCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

        CORE_ASSERT(vkAllocateCommandBuffers(device_.GetDevice(), &allocInfo, commandBuffers_.data()) == VK_SUCCESS, "Failed to allocate command buffers!")
    }

    void VulkanRenderer::FreeCommandBuffers() {

    }

    void VulkanRenderer::RecreateSwapChain() {
        vkDeviceWaitIdle(device_.GetDevice());

        if (engineSwapChain_ == nullptr) {
            engineSwapChain_ = std::make_unique<VulkanSwapChain>(device_, VkExtent2D{window_.GetWidth(), window_.GetHeight()});
        } else {
            engineSwapChain_ = std::make_unique<VulkanSwapChain>(device_, VkExtent2D{window_.GetWidth(), window_.GetHeight()}, engineSwapChain_.get());
        }
    }
}
//
// Created by standa on 5.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <renderer/GraphicsContext.h>
#include <platform/vulkan/VulkanDevice.h>
#include <platform/vulkan/VulkanRenderer.h>
#include <platform/vulkan/VulkanDescriptors.h>

namespace VulkanEngine {

    class VulkanContext : public GraphicsContext {
    public:
        explicit VulkanContext(const Window& window);

        void Init() override;
        void InitImGuiVulkan() override;

        VkCommandBuffer BeginFrame() override;
        void EndFrame(VkCommandBuffer commandBuffer) override;
    private:
        const Window& window_;

        std::unique_ptr<VulkanDevice> vulkanDevice_;
        std::unique_ptr<VulkanRenderer> vulkanRenderer_;
        std::unique_ptr<VulkanDescriptorPool> globalPool_;
    };
}
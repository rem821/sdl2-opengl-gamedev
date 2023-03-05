//
// Created by standa on 5.3.23.
//
#pragma once

namespace VulkanEngine {

    class GraphicsContext {
    public:
        virtual void Init() = 0;

        virtual void InitImGuiVulkan() = 0;

        virtual VkCommandBuffer BeginFrame() = 0;

        virtual void EndFrame(VkCommandBuffer commandBuffer) = 0;
    };
}
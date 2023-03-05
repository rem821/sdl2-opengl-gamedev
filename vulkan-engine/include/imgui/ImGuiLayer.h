//
// Created by standa on 4.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>

namespace VulkanEngine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        void Begin();
        void End(VkCommandBuffer commandBuffer);

    private:
        float time_ = 0.0f;
    };
}
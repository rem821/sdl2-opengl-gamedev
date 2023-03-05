//
// Created by standa on 4.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>

namespace VulkanEngine {

    class ENGINE_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(VkCommandBuffer commandBuffer) override;
        void OnEvent(Event& event) override;
    private:
        float time_ = 0.0f;
    };
}
//
// Created by standa on 4.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <vulkan/vulkan.h>

namespace VulkanEngine {

    class ENGINE_API Layer {
    public:
        explicit Layer(std::string name = "Layer");

        virtual ~Layer();

        virtual void OnAttach() {}

        virtual void OnDetach() {}

        virtual void OnUpdate(VkCommandBuffer commandBuffer) {}

        virtual void OnEvent(Event &event) {}

        [[nodiscard]] inline const std::string &GetName() const { return debugName_; }

    private:
        std::string debugName_;
    };
}
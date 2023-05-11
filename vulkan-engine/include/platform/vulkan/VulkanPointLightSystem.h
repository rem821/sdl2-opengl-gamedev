//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <platform/vulkan/VulkanDevice.h>
#include <platform/vulkan/VulkanFrameInfo.h>
#include <platform/vulkan/VulkanPipeline.h>
#include "rendering/Camera.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace VulkanEngine {
    struct PointLightPushConstants {
        glm::vec3 position{};
        alignas(16) glm::vec4 color{};
        float radius;
    };

    class VulkanPointLightSystem {

    public:
        VulkanPointLightSystem(VulkanDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~VulkanPointLightSystem();

        VulkanPointLightSystem(const VulkanPointLightSystem &) = delete;

        VulkanPointLightSystem &operator=(const VulkanPointLightSystem &) = delete;

        void Update(FrameInfo &frameInfo, GlobalUbo &ubo);

        void Render(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void CreatePipeline(VkRenderPass renderPass);

        VulkanDevice &engineDevice_;

        std::unique_ptr<VulkanPipeline> enginePipeline;
        VkPipelineLayout pipelineLayout;
    };
}


//
// Created by standa on 10.5.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <platform/vulkan/VulkanDevice.h>
#include <platform/vulkan/VulkanPipeline.h>
#include <platform/vulkan/VulkanFrameInfo.h>
#include <glm/glm.hpp>

namespace VulkanEngine {

    struct SimplePushConstants {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    class VulkanRenderSystem {
    public:
        VulkanRenderSystem(VulkanDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode);

        ~VulkanRenderSystem();

        VulkanRenderSystem(const VulkanRenderSystem &) = delete;

        VulkanRenderSystem &operator=(const VulkanRenderSystem &) = delete;

        void RenderGameObjects(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void CreatePipeline(VkRenderPass renderPass, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode);

        VulkanDevice &engineDevice_;

        bool isWireFrame_ = false;

        std::unique_ptr<VulkanPipeline> enginePipeline_;
        VkPipelineLayout pipelineLayout_{};
    };
}
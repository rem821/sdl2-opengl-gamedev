//
// Created by standa on 10.5.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <platform/vulkan/VulkanDevice.h>
#include <platform/vulkan/VulkanModel.h>

namespace VulkanEngine {

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo operator=(const PipelineConfigInfo &) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo viewportInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class VulkanPipeline {
    public:
        VulkanPipeline(VulkanDevice &engineDevice, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline &) = delete;
        VulkanPipeline operator=(const VulkanPipeline &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        static void DefaultPipelineConfig(PipelineConfigInfo &configInfo);
        static void EnableAlphaBlending(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char> ReadFile(const std::string &filePath);
        void CreateGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        void CreateShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        VulkanDevice &engineDevice_;
        VkPipeline graphicsPipeline_;
        VkShaderModule vertShaderModule_;
        VkShaderModule fragShaderModule_;
    };
}
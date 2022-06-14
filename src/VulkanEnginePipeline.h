//
// Created by Stanislav Svědiroh on 12.06.2022.
//
#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "fmt/core.h"
#include "VulkanEngineDevice.h"
#include "VulkanEngineModel.h"

struct PipelineConfigInfo {
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo operator=(const PipelineConfigInfo &) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class VulkanEnginePipeline {
public:
    VulkanEnginePipeline(VulkanEngineDevice &engineDevice, const std::string &vertFilePath, const std::string &fragFilePath,
                         const PipelineConfigInfo &configInfo);
    ~VulkanEnginePipeline();

    VulkanEnginePipeline(const VulkanEnginePipeline &) = delete;
    VulkanEnginePipeline operator=(const VulkanEnginePipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    static void defaultPipelineConfig(PipelineConfigInfo &configInfo);

private:
    static std::vector<char> readFile(const std::string &filePath);
    void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath,
                                const PipelineConfigInfo &configInfo);
    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    VulkanEngineDevice &engineDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
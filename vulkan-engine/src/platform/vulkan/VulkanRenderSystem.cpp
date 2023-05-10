//
// Created by standa on 10.5.23.
//

#include <platform/vulkan/VulkanRenderSystem.h>

namespace VulkanEngine {

    VulkanRenderSystem::VulkanRenderSystem(VulkanDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkPolygonMode polygonMode,
                                           VkCullModeFlagBits cullMode) : engineDevice_{device} {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline(renderPass, polygonMode, cullMode);

        isWireFrame_ = polygonMode == VK_POLYGON_MODE_LINE;
    }

    VulkanRenderSystem::~VulkanRenderSystem() {
        vkDestroyPipelineLayout(engineDevice_.GetDevice(), pipelineLayout_, nullptr);
    }

    void VulkanRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange = {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstants);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(engineDevice_.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void
    VulkanRenderSystem::CreatePipeline(VkRenderPass renderPass, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode) {
        assert(pipelineLayout_ != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig = {};
        VulkanPipeline::DefaultPipelineConfig(pipelineConfig);
        pipelineConfig.rasterizationInfo.polygonMode = polygonMode;
        pipelineConfig.rasterizationInfo.cullMode = cullMode;

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout_;

        std::string fragment_shader;
        if (polygonMode == VK_POLYGON_MODE_LINE) {
            fragment_shader = "shaders/line_shader.frag.spv";
        } else {
            fragment_shader = "shaders/shader.frag.spv";
        }
        enginePipeline_ = std::make_unique<VulkanPipeline>(engineDevice_, "shaders/shader.vert.spv", fragment_shader, pipelineConfig);
    }

    void VulkanRenderSystem::RenderGameObjects(FrameInfo &frameInfo) {
        enginePipeline_->Bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    }

}
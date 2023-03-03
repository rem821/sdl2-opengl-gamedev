//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#include "../../include/systems/SimpleRenderSystem.h"

SimpleRenderSystem::SimpleRenderSystem(VulkanEngineDevice &device, VkRenderPass renderPass,
                                       VkDescriptorSetLayout globalSetLayout, VkPolygonMode polygonMode,
                                       VkCullModeFlagBits cullMode)
        : engineDevice{device} {
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass, polygonMode, cullMode);

    isWireFrame = polygonMode == VK_POLYGON_MODE_LINE;
}

SimpleRenderSystem::~SimpleRenderSystem() {
    vkDestroyPipelineLayout(engineDevice.getDevice(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
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

    if (vkCreatePipelineLayout(engineDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void
SimpleRenderSystem::createPipeline(VkRenderPass renderPass, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig = {};
    VulkanEnginePipeline::defaultPipelineConfig(pipelineConfig);
    pipelineConfig.rasterizationInfo.polygonMode = polygonMode;
    pipelineConfig.rasterizationInfo.cullMode = cullMode;

    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;

    std::string fragment_shader;
    if (polygonMode == VK_POLYGON_MODE_LINE) {
        fragment_shader = "shaders/line_shader.frag.spv";
    } else {
        fragment_shader = "shaders/shader.frag.spv";
    }
    enginePipeline = std::make_unique<VulkanEnginePipeline>(engineDevice, "shaders/shader.vert.spv",
                                                            fragment_shader, pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo) {
    enginePipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                            &frameInfo.globalDescriptorSet, 0, nullptr);

    std::vector<id_t> deleteIds = {};
    for (auto &kv: frameInfo.gameObjects) {
        auto &obj = kv.second;
        if (obj.model == nullptr) continue;
        if (isWireFrame && obj.renderMode == GameObject::RENDER_MODE_FILLED) continue;
        if ((!isWireFrame && obj.renderMode == GameObject::RENDER_MODE_BOTH) ||
            (!isWireFrame && obj.renderMode == GameObject::RENDER_MODE_WIREFRAME))
            continue;
        if (!obj.isActive) {
            if (obj.getIsInvalidated()) {
                if (obj.shouldBeDestroyed()) {
                    deleteIds.push_back(obj.getId());
                }
            }
            continue;
        }

        SimplePushConstants push = {};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();

        vkCmdPushConstants(
                frameInfo.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstants),
                &push
        );
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }

    for (const auto &deleteId: deleteIds) {
        frameInfo.gameObjects.erase(frameInfo.gameObjects.find(deleteId));
    }
}

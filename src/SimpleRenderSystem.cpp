//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#include "SimpleRenderSystem.h"

SimpleRenderSystem::SimpleRenderSystem(VulkanEngineDevice &device, VkRenderPass renderPass): engineDevice{device} {
    createPipelineLayout();
    createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() { vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr); };

void SimpleRenderSystem::createPipelineLayout() {
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig = {};
    VulkanEnginePipeline::defaultPipelineConfig(pipelineConfig);

    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    enginePipeline = std::make_unique<VulkanEnginePipeline>(engineDevice, "shaders/shader.vert.spv",
                                                            "shaders/shader.frag.spv", pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera &camera) {
    enginePipeline->bind(commandBuffer);

    auto projectionView = camera.getProjection() * camera.getView();

    for (auto &obj: gameObjects) {
        SimplePushConstantData push = {};
        push.color = obj.color;
        push.transform = projectionView * obj.transform.mat4();

        vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
        );
        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}

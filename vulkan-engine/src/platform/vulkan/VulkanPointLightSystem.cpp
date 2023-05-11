//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#include "platform/vulkan/VulkanPointLightSystem.h"

namespace VulkanEngine {
    VulkanPointLightSystem::VulkanPointLightSystem(VulkanDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : engineDevice_{device} {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline(renderPass);
    }

    VulkanPointLightSystem::~VulkanPointLightSystem() { vkDestroyPipelineLayout(engineDevice_.GetDevice(), pipelineLayout, nullptr); }

    void VulkanPointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange = {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstants);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(engineDevice_.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void VulkanPointLightSystem::CreatePipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig = {};
        VulkanPipeline::DefaultPipelineConfig(pipelineConfig);
        VulkanPipeline::EnableAlphaBlending(pipelineConfig);

        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.attributeDescriptions.clear();

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        enginePipeline = std::make_unique<VulkanPipeline>(engineDevice_, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv", pipelineConfig);
    }

    void VulkanPointLightSystem::Render(FrameInfo &frameInfo) {
        // sort lights
        std::map<float, VulkanGameObject::id_t> sorted;
        for (auto &kv: frameInfo.gameObjects) {
            auto &obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            auto offset = frameInfo.camera.GetPosition() - obj.transform.translation;
            float disSquared = glm::dot(offset, offset);
            sorted[disSquared] = obj.GetId();
        }


        enginePipeline->Bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        // iterate through sorted lights in reverse order
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            auto &obj = frameInfo.gameObjects.at(it->second);

            PointLightPushConstants push{};
            push.position = obj.transform.translation;
            push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
            push.radius = obj.transform.scale.x;

            vkCmdPushConstants(
                    frameInfo.commandBuffer,
                    pipelineLayout,
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                    0,
                    sizeof(PointLightPushConstants),
                    &push
            );
            vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
        }
    }

    void VulkanPointLightSystem::Update(FrameInfo &frameInfo, GlobalUbo &ubo) {
        int lightIndex = 0;

        for (auto &kv: frameInfo.gameObjects) {
            auto &obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            //copy light to ubo
            ubo.pointLights[lightIndex].position = obj.transform.translation;
            ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
            lightIndex += 1;
        }
        ubo.numLights = lightIndex;
    }
}
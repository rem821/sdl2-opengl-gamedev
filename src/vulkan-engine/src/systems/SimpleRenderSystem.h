//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include "SDL.h"
#include "../rendering/VulkanEngineDevice.h"
#include "../rendering/VulkanEnginePipeline.h"
#include "../rendering/GameObject.h"
#include "../rendering/Camera.h"
#include "../rendering/VulkanEngineFrameInfo.h"

#include "../rendering/ChunkDeserializer.h"
#include "fmt/include/fmt/core.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

struct SimplePushConstants {
    glm::mat4 modelMatrix{1.f};
    glm::mat4 normalMatrix{1.f};
};

class SimpleRenderSystem {

public:
    SimpleRenderSystem(VulkanEngineDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode);
    ~SimpleRenderSystem();
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode);

    bool isWireFrame = false;

    VulkanEngineDevice &engineDevice;

    std::unique_ptr<VulkanEnginePipeline> enginePipeline;
    VkPipelineLayout pipelineLayout{};
};
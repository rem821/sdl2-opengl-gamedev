//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "../rendering/VulkanEngineDevice.h"
#include "../rendering/VulkanEnginePipeline.h"
#include "../rendering/GameObject.h"
#include "../rendering/Camera.h"
#include "../rendering/VulkanEngineFrameInfo.h"

#include "../rendering/Map.h"
#include <fmt/core.h>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct PointLightPushConstants {
    glm::vec3 position{};
    alignas(16) glm::vec4 color{};
    float radius;
};

class PointLightSystem {

public:
    PointLightSystem(VulkanEngineDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~PointLightSystem();
    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void update(FrameInfo &frameInfo, GlobalUbo &ubo);
    void render(FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    VulkanEngineDevice &engineDevice;

    std::unique_ptr<VulkanEnginePipeline> enginePipeline;
    VkPipelineLayout pipelineLayout;
};
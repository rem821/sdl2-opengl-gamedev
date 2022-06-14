//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "VulkanEngineDevice.h"
#include "VulkanEnginePipeline.h"
#include "GameObject.h"
#include "Camera.h"
#include "CustomTypes.h"
#include "TextureManager.h"
#include "Map.h"
#include "fmt/core.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

struct SimplePushConstantData {
    glm::mat4 transform{1.f};
    alignas(16) glm::vec3 color;
};

class SimpleRenderSystem {

public:
    SimpleRenderSystem(VulkanEngineDevice &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera &camera);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    VulkanEngineDevice &engineDevice;

    std::unique_ptr<VulkanEnginePipeline> enginePipeline;
    VkPipelineLayout pipelineLayout;
};
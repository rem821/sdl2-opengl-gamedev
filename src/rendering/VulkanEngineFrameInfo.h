//
// Created by Stanislav Svědiroh on 15.06.2022.
//
#pragma once

#include "Camera.h"
#include "GameObject.h"

#include <vulkan/vulkan.h>

# define MAX_LIGHTS 10

struct PointLight {
    glm::vec3 position{};
    alignas(16) glm::vec4 color{};
};

struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::mat4 inverseView{1.f};
    glm::vec4 ambientLightColor{0.609f, 0.18f, 0.207f, .08f}; // w is intensity
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::Map &gameObjects;
};
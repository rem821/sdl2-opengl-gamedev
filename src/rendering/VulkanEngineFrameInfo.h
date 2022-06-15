//
// Created by Stanislav Svědiroh on 15.06.2022.
//
#pragma once

#include "Camera.h"
#include "GameObject.h"

#include <vulkan/vulkan.h>

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    GameObject::Map &gameObjects;
};
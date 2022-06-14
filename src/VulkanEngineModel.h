//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#pragma once

#include "VulkanEngineDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"

#include <vector>
#include <cassert>

class VulkanEngineModel {
public:

    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };

    VulkanEngineModel(VulkanEngineDevice &core, const VulkanEngineModel::Builder &builder);
    ~VulkanEngineModel();

    VulkanEngineModel(const VulkanEngineModel &) = delete;
    VulkanEngineModel operator=(const VulkanEngineModel &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffer(const std::vector<uint32_t> &indices);

    VulkanEngineDevice &engineDevice;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t indexCount;
};
//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "VulkanEngineUtils.h"
#include "VulkanEngineBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "fmt/core.h"

#include <vector>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <unordered_map>

class VulkanEngineModel {
public:

    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex &other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string &filepath);
    };

    VulkanEngineModel(VulkanEngineDevice &core, const VulkanEngineModel::Builder &builder);
    ~VulkanEngineModel();

    VulkanEngineModel(const VulkanEngineModel &) = delete;
    VulkanEngineModel operator=(const VulkanEngineModel &) = delete;

    static std::unique_ptr<VulkanEngineModel> createModelFromFile(VulkanEngineDevice &device, const std::string &filepath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffer(const std::vector<uint32_t> &indices);

    VulkanEngineDevice &engineDevice;

    std::unique_ptr<VulkanEngineBuffer> vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<VulkanEngineBuffer> indexBuffer;
    uint32_t indexCount;
};
//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <platform/vulkan/VulkanDevice.h>
#include <platform/vulkan/VulkanBuffer.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

namespace VulkanEngine {

    class VulkanModel {
    public:

        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
            ~Vertex() = default;
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void LoadModel(const std::string &filepath);
        };

        VulkanModel(VulkanDevice &device, const VulkanModel::Builder &builder);

        ~VulkanModel();

        VulkanModel(const VulkanModel &) = delete;

        VulkanModel operator=(const VulkanModel &) = delete;

        static std::unique_ptr<VulkanModel> CreateModelFromFile(VulkanDevice &device, const std::string &filepath);

        void Bind(VkCommandBuffer commandBuffer);

        void Draw(VkCommandBuffer commandBuffer);

        [[nodiscard]] uint32_t GetVertexCount() const { return vertexCount_; };

    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        void CreateIndexBuffer(const std::vector<uint32_t> &indices);

        VulkanDevice &engineDevice_;

        std::unique_ptr<VulkanBuffer> vertexBuffer_;
        uint32_t vertexCount_;

        bool hasIndexBuffer_ = false;
        std::unique_ptr<VulkanBuffer> indexBuffer_;
        uint32_t indexCount_;
    };
}

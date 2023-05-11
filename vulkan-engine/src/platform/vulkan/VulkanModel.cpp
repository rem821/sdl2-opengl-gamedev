//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#include <platform/vulkan/VulkanModel.h>

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

template<typename T, typename... Rest>
void hashCombine(std::size_t &seed, const T &v, const Rest&... rest) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

namespace std {
    template<>
    struct hash<VulkanEngine::VulkanModel::Vertex> {
        size_t operator()(VulkanEngine::VulkanModel::Vertex const &vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace VulkanEngine {

    VulkanModel::VulkanModel(VulkanDevice &device, const VulkanModel::Builder &builder) : engineDevice_{device} {
        CreateVertexBuffer(builder.vertices);
        CreateIndexBuffer(builder.indices);
    }

    VulkanModel::~VulkanModel() = default;

    void VulkanModel::Bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer_->GetBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer_) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer_->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void VulkanModel::Draw(VkCommandBuffer commandBuffer) const {
        if (hasIndexBuffer_) {
            vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount_, 1, 0, 0);
        }
    }

    void VulkanModel::CreateVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount_ = static_cast<uint32_t>(vertices.size());
        assert(vertexCount_ >= 3 && " Vertex count must be at least 3");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount_;
        uint32_t vertexSize = sizeof vertices[0];

        VulkanBuffer stagingBuffer{
                engineDevice_,
                vertexSize,
                vertexCount_,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *) vertices.data());

        vertexBuffer_ = std::make_unique<VulkanBuffer>(
                engineDevice_,
                vertexSize,
                vertexCount_,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        engineDevice_.CopyBuffer(stagingBuffer.GetBuffer(), vertexBuffer_->GetBuffer(), bufferSize);
    }

    void VulkanModel::CreateIndexBuffer(const std::vector<uint32_t> &indices) {
        indexCount_ = static_cast<uint32_t>(indices.size());
        hasIndexBuffer_ = indexCount_ > 0;
        if (!hasIndexBuffer_) return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount_;
        uint32_t indexSize = sizeof indices[0];

        VulkanBuffer stagingBuffer{
                engineDevice_,
                indexSize,
                indexCount_,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *) indices.data());

        indexBuffer_ = std::make_unique<VulkanBuffer>(
                engineDevice_,
                indexSize,
                indexCount_,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        engineDevice_.CopyBuffer(stagingBuffer.GetBuffer(), indexBuffer_->GetBuffer(), bufferSize);

    }

    std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::GetBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::GetAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
        attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

        return attributeDescriptions;
    }

    std::unique_ptr<VulkanModel> VulkanModel::CreateModelFromFile(VulkanDevice &device, const std::string &filepath) {
        Builder builder{};
        builder.LoadModel(filepath);

        CORE_INFO("Loading model with {} vertices\n", builder.vertices.size());
        return std::make_unique<VulkanModel>(device, builder);
    }

    void VulkanModel::Builder::LoadModel(const std::string &filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices;
        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };

                    vertex.color = {
                            attrib.colors[3 * index.vertex_index + 0],
                            attrib.colors[3 * index.vertex_index + 1],
                            attrib.colors[3 * index.vertex_index + 2],
                    };
                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords[3 * index.texcoord_index + 0],
                            attrib.texcoords[3 * index.texcoord_index + 1],
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}


#include "Block.h"

VulkanEngineModel::Builder Block::getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation, glm::vec3 size) {
    VulkanEngineModel::Builder builder = VulkanEngineModel::Builder{};

    if (orientation == TOP || orientation == BOTTOM) {
        int normal;
        if (orientation == TOP) {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = -1.0f;
        } else {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {-(size.x * 0.5f) + x_tr, 0.0f + y_tr, -(size.z * 0.5f) + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, normal, 0.0f};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-(size.x * 0.5f) + x_tr, 0.0f + y_tr, (size.z * 0.5f) + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, normal, 0.0f};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {(size.x * 0.5f) + x_tr, 0.0f + y_tr, (size.z * 0.5f) + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, normal, 0.0f};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {(size.x * 0.5f) + x_tr, 0.0f + y_tr, -(size.z * 0.5f) + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, normal, 0.0f};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == FRONT || orientation == BACK) {
        int normal;
        if (orientation == FRONT) {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = -1.0f;
        } else {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {-(size.x * 0.5f) + x_tr, (size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-(size.x * 0.5f) + x_tr, -(size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {(size.x * 0.5f) + x_tr, -(size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {(size.x * 0.5f) + x_tr, (size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == LEFT || orientation == RIGHT) {
        int normal;
        if (orientation == LEFT) {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = -1.0f;
        } else {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {0.0f + x_tr, (size.y * 0.5f) + y_tr, -(size.z * 0.5f) + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {0.0f + x_tr, -(size.y * 0.5f) + y_tr, -(size.z * 0.5f) + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {0.0f + x_tr, -(size.y * 0.5f) + y_tr, (size.z * 0.5f) + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {0.0f + x_tr, (size.y * 0.5f) + y_tr, (size.z * 0.5f) + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    }

    return builder;
}


VulkanEngineModel::Builder Block::getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left, bool right, bool top, bool bottom, bool front, bool back) {
    VulkanEngineModel::Builder cubeFaces;

    int i = 0;
    if (left) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y - (0.5f * size.x), world_pos.z, world_pos.x, FaceOrientation::LEFT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }

        i += 4;
    }
    if (right) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y + (0.5f * size.x), world_pos.z, world_pos.x, FaceOrientation::RIGHT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (top) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z - (0.5f * size.y), world_pos.x, FaceOrientation::TOP, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (bottom) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z + (0.5f * size.y), world_pos.x, FaceOrientation::BOTTOM, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (front) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z, world_pos.x - (0.5f * size.z), FaceOrientation::FRONT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (back) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z, world_pos.x + (0.5f * size.z), FaceOrientation::BACK, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    return cubeFaces;
}
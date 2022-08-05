#include "Block.h"

VulkanEngineModel::Builder Block::getFaceVertices(glm::vec3 pos, FaceOrientation orientation, glm::vec3 size, glm::vec3 color) {
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
        vert1.position = {pos.x, pos.y, pos.z};
        vert1.color = color;
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, normal, 0.0f};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {pos.x, pos.y, pos.z + size.z};
        vert2.color = color;
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, normal, 0.0f};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {pos.x + size.x, pos.y, pos.z + size.z};
        vert3.color = color;
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, normal, 0.0f};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {pos.x + size.x, pos.y, pos.z};
        vert4.color = color;
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, normal, 0.0f};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == LEFT || orientation == RIGHT) {
        int normal;
        if (orientation == RIGHT) {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = -1.0f;
        } else {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {pos.x, pos.y + size.y, pos.z};
        vert1.color = color;
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {pos.x, pos.y, pos.z};
        vert2.color = color;
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {pos.x + size.x, pos.y, pos.z};
        vert3.color = color;
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {pos.x + size.x, pos.y + size.y, pos.z};
        vert4.color = color;
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == FRONT || orientation == BACK) {
        int normal;
        if (orientation == BACK) {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = -1.0f;
        } else {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {pos.x, pos.y + size.y, pos.z};
        vert1.color = color;
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {pos.x, pos.y, pos.z};
        vert2.color = color;
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {pos.x, pos.y, pos.z + size.z};
        vert3.color = color;
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {pos.x, pos.y + size.y, pos.z + size.z};
        vert4.color = color;
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    }

    return builder;
}


VulkanEngineModel::Builder Block::getCubeFaces(glm::vec3 world_pos, glm::vec3 size, glm::vec3 color, bool left, bool right, bool top, bool bottom, bool front, bool back) {
    VulkanEngineModel::Builder cubeFaces;

    int i = 0;
    if (left) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x, world_pos.y, world_pos.z}),
                                                              FaceOrientation::LEFT, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }

        i += 4;
    }
    if (right) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x + size.x, world_pos.y, world_pos.z}),
                                                              FaceOrientation::RIGHT, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (top) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x, world_pos.y, world_pos.z + size.z}),
                                                              FaceOrientation::TOP, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (bottom) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x, world_pos.y, world_pos.z}),
                                                              FaceOrientation::BOTTOM, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (front) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x, world_pos.y, world_pos.z}),
                                                              FaceOrientation::FRONT, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (back) {
        VulkanEngineModel::Builder vertices = getFaceVertices(fromWorldToCamera({world_pos.x, world_pos.y + size.y, world_pos.z}),
                                                              FaceOrientation::BACK, fromWorldToCamera(size), color);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
    }

    return cubeFaces;
}
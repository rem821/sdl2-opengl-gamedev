//
// Created by Stanislav Svědiroh on 26.06.2022.
//
#include "Cube.h"

Cube::Cube(VulkanEngineDevice &device) : engineDevice{device} {

}

VulkanEngineModel::Builder Cube::getVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation) {
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
        vert1.position = {-1.0f + x_tr, 0.0f + y_tr, -1.0f + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, normal, 0.0f};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-1.0f + x_tr, 0.0f + y_tr, 1.0f + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, normal, 0.0f};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {1.0f + x_tr, 0.0f + y_tr, 1.0f + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, normal, 0.0f};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {1.0f + x_tr, 0.0f + y_tr, -1.0f + z_tr};
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
        vert1.position = {-1.0f + x_tr, 1.0f + y_tr, 0.0f + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-1.0f + x_tr, -1.0f + y_tr, 0.0f + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {1.0f + x_tr, -1.0f + y_tr, 0.0f + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {1.0f + x_tr, 1.0f + y_tr, 0.0f + z_tr};
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
        vert1.position = {0.0f + x_tr, 1.0f + y_tr, -1.0f + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {0.0f + x_tr, -1.0f + y_tr, -1.0f + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {0.0f + x_tr, -1.0f + y_tr, 1.0f + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {0.0f + x_tr, 1.0f + y_tr, 1.0f + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    }

    return builder;
}


VulkanEngineModel::Builder
Cube::getCubeFaces(float world_x, float world_y, float world_z, bool left, bool right, bool top, bool bottom, bool front, bool back) {
    VulkanEngineModel::Builder cubeFaces;

    int i = 0;
    if (left) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f) + -1.0f, (world_z * 2.0f), (world_x * 2.0f), FaceOrientation::LEFT);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }

        i += 4;
    }
    if (right) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f) + 1.0f, (world_z * 2.0f), (world_x * 2.0f), FaceOrientation::RIGHT);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (top) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f), (world_z * 2.0f) + -1.0f, (world_x * 2.0f), FaceOrientation::TOP);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (bottom) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f), (world_z * 2.0f) + 1.0f, (world_x * 2.0f), FaceOrientation::BOTTOM);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (front) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f), (world_z * 2.0f), (world_x * 2.0f) + -1.0f, FaceOrientation::FRONT);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (back) {
        VulkanEngineModel::Builder vertices = getVertices((world_y * 2.0f), (world_z * 2.0f), (world_x * 2.0f) + 1.0f, FaceOrientation::BACK);
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

//
// Created by Stanislav Svědiroh on 26.06.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"
#include "VulkanEngineModel.h"

#include <glm/glm.hpp>
#include <vector>
#include <fmt/core.h>

class Cube {
public:
    enum FaceOrientation {
        LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK
    };

    Cube(VulkanEngineDevice &device);
    Cube(const Cube &) = delete;
    Cube &operator=(const Cube &) = delete;
    Cube(Cube &&) = default;
    Cube &operator=(Cube &&) = default;

    VulkanEngineModel::Builder getVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation);
    VulkanEngineModel::Builder
    getCubeFaces(float world_x, float world_y, float world_z, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);
private:

    VulkanEngineDevice &engineDevice;
    std::shared_ptr<VulkanEngineModel> quad = VulkanEngineModel::createModelFromFile(engineDevice, "models/quad.obj");
};
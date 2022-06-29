//
// Created by Stanislav Svědiroh on 26.06.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"

class Cube {
public:
    Cube(VulkanEngineDevice &device);
    Cube(const Cube &) = delete;
    Cube &operator=(const Cube &) = delete;
    Cube(Cube &&) = default;
    Cube &operator=(Cube &&) = default;

    GameObject::Map getCubeFaces(float world_x, float world_y, float world_z, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true, bool back = true);
private:

    VulkanEngineDevice &engineDevice;
    std::shared_ptr<VulkanEngineModel> quad = VulkanEngineModel::createModelFromFile(engineDevice, "models/quad.obj");

};
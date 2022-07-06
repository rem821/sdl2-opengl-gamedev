#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"

#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define MAP_HEIGHT 400
#define MAP_WIDTH 400
#define MAP_DEPTH 100

class TerrainDeserializer {
public:
    enum FaceOrientation {
        LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK
    };

    TerrainDeserializer(VulkanEngineDevice &engineDevice);
    ~TerrainDeserializer();

    GameObject getMapBlocks();

private:
    void loadLevel(int level);

    uint32_t& getMapBlock(int x, int y, int z);
    VulkanEngineModel::Builder getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation);
    VulkanEngineModel::Builder
    getCubeFaces(float world_x, float world_y, float world_z, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);

    VulkanEngineDevice &device;

    uint32_t map_blocks[MAP_DEPTH * MAP_WIDTH * MAP_HEIGHT];
    int level[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
};
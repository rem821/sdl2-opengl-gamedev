#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"

#include <glm/glm.hpp>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

#define MAP_HEIGHT 1088
#define MAP_WIDTH 1920
#define MAP_DEPTH 256
#define CHUNK_SIZE 32

class TerrainDeserializer {
public:
    enum FaceOrientation {
        LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK
    };

    TerrainDeserializer(VulkanEngineDevice &engineDevice);
    ~TerrainDeserializer();

    GameObject getMapBlocks();

    GameObject getChunkBorders();

private:
    void loadLevel(int chunk_x, int chunk_y);

    uint32_t &getMapBlock(int x, int y, int z);
    VulkanEngineModel::Builder getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation, glm::vec3 size);
    VulkanEngineModel::Builder
    getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);

    VulkanEngineDevice &device;

    uint32_t map_blocks[MAP_DEPTH * MAP_WIDTH * MAP_HEIGHT];
};
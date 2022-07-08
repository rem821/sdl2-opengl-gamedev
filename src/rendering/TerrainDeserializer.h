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
#include <unordered_map>
#include <exception>

#define MAP_HEIGHT 544
#define MAP_WIDTH 960
#define MAP_DEPTH 256
#define CHUNK_SIZE 32


class TerrainDeserializer {
public:
    using chunk_id = std::string;
    using Block = unsigned char;
    using Chunk = unsigned char[CHUNK_SIZE * CHUNK_SIZE * MAP_DEPTH];
    using ChunkMap = std::unordered_map<chunk_id, Chunk>;

    enum FaceOrientation {
        LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK
    };

    TerrainDeserializer(VulkanEngineDevice &engineDevice);
    ~TerrainDeserializer();

    chunk_id getChunkId(glm::uvec2 position) { return fmt::format("{}_{}", position.x, position.y); }

    GameObject getChunkGameObject(glm::uvec2 position);

    GameObject getChunkBorders();

private:
    void deserializeChunk(glm::uvec2 chunk_pos);

    Block &getChunkBlock(const chunk_id &id, glm::uvec3 pos);
    VulkanEngineModel::Builder getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation, glm::vec3 size);
    VulkanEngineModel::Builder
    getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);

    VulkanEngineDevice &device;

    ChunkMap chunks;
};
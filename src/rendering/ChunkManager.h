//
// Created by Stanislav Svědiroh on 09.07.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "ChunkDeserializer.h"
#include "Block.h"

#include <unordered_map>
#include <glm/glm.hpp>
#include <fmt/core.h>
#include <cassert>

#define CHUNK_SIZE 32
#define CHUNK_DEPTH 256

class ChunkManager {
public:
    using chunk_id = std::string;
    using Chunk = Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH];
    using ChunkMap = std::unordered_map<chunk_id, Chunk>;

    ChunkManager(VulkanEngineDevice &device) : _device{device} {};
    ~ChunkManager() = default;

    ChunkManager(const ChunkManager &) = delete;
    ChunkManager &operator=(const ChunkManager &) = delete;


    Block &getChunkBlock(const chunk_id &id, glm::uvec3 pos) { return _chunks[id][pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    static Block &getChunkBlock(Chunk &chunk, glm::uvec3 pos) { return chunk[pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    Chunk &getChunk(const chunk_id &id) { return _chunks[id]; };

    void populateChunk(ChunkManager::Chunk &chunk, ChunkDeserializer::RawChunkData rawData);

    static chunk_id getChunkId(glm::uvec2 position) { return fmt::format("{}_{}", position.x, position.y); };

    GameObject getChunkGameObject(glm::uvec2 position);

    GameObject getChunkBorders(glm::vec2 chunk_pos);

private:

    std::unordered_map<chunk_id, bool> isChunkLoaded{};

    VulkanEngineDevice &_device;
    ChunkDeserializer terrainDeserializer{};

    ChunkMap _chunks;
};

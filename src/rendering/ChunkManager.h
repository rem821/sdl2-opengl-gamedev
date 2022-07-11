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
#include <vector>
#include <thread>

#define CHUNK_SIZE 32
#define CHUNK_DEPTH 256

class ChunkManager {
public:
    using chunk_id = std::string;
    using Chunk = Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH];
    using ChunkMap = std::unordered_map<chunk_id, Chunk>;
    using ActiveChunkMap = std::unordered_map<chunk_id, GameObject>;

    ChunkManager(VulkanEngineDevice &device) : _device{device} {};
    ~ChunkManager() = default;

    ChunkManager(const ChunkManager &) = delete;
    ChunkManager &operator=(const ChunkManager &) = delete;


    Block &getChunkBlock(const chunk_id &id, glm::uvec3 pos) { return _chunks[id][pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    static Block &getChunkBlock(Chunk &chunk, glm::uvec3 pos) { return chunk[pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    Chunk &getChunk(const chunk_id &id) { return _chunks[id]; };

    static chunk_id getChunkId(glm::uvec2 position) { return fmt::format("{}_{}", position.x, position.y); };

    void loadChunksAroundPlayer(glm::uvec2 player_pos, uint32_t distance);

    ActiveChunkMap &getActiveChunks() { return _activeChunks; };

    glm::uvec2 getChunkFromPlayerPos(glm::uvec2 player_pos);

private:

    void generateChunkGameObject(glm::uvec2 position);

    GameObject getChunkBorders(glm::vec2 chunk_pos);

    void populateChunk(ChunkManager::Chunk &chunk, ChunkDeserializer::RawChunkData rawData);

    VulkanEngineDevice &_device;
    ChunkDeserializer terrainDeserializer{};

    ChunkMap _chunks;
    ActiveChunkMap _activeChunks;

    std::unordered_map<glm::uvec2, VulkanEngineModel::Builder> chunkPrefabs;
};

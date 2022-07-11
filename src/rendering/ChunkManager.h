//
// Created by Stanislav Svědiroh on 09.07.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "ChunkDeserializer.h"
#include "Block.h"
#include "../threading/BS_thread_pool.h"

#include <unordered_map>
#include <glm/glm.hpp>
#include <fmt/core.h>
#include <cassert>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>

#define CHUNK_SIZE 32
#define CHUNK_DEPTH 256

class ChunkManager {
public:
    using chunk_id = std::string;
    using Chunk = Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH];
    using ActiveChunkMap = std::unordered_map<chunk_id, Chunk>;
    using VisibleChunkMap = std::unordered_map<chunk_id, GameObject>;
    using ChunkPrefab = std::pair<glm::uvec2, VulkanEngineModel::Builder>;

    ChunkManager(VulkanEngineDevice &device) : _device{device} {};
    ~ChunkManager() = default;

    ChunkManager(const ChunkManager &) = delete;
    ChunkManager &operator=(const ChunkManager &) = delete;


    Block &getChunkBlock(const chunk_id &id, glm::uvec3 pos) { return _activeChunks[id][pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    static Block &getChunkBlock(Chunk &chunk, glm::uvec3 pos) { return chunk[pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * CHUNK_DEPTH]; };

    Chunk &getChunk(const chunk_id &id) { return _activeChunks[id]; };

    static chunk_id getChunkId(glm::uvec2 position) { return fmt::format("{}_{}", position.x, position.y); };

    void loadChunksAroundPlayerAsync(glm::uvec2 player_pos, uint32_t distance);

    VisibleChunkMap &getVisibleChunks();

    GameObject getChunkBorders(glm::vec2 chunk_pos);

    static glm::uvec2 getChunkFromPlayerPos(glm::uvec2 player_pos);

private:

    ChunkPrefab generateChunkGameObject(const glm::uvec2 position);

    void populateChunk(ChunkManager::Chunk &chunk, ChunkDeserializer::RawChunkData rawData);

    VulkanEngineDevice &_device;
    ChunkDeserializer terrainDeserializer{};

    BS::thread_pool pool;
    std::mutex mut;

    ActiveChunkMap _activeChunks;
    VisibleChunkMap _visibleChunks;
    std::vector<chunk_id> _requestedChunks;

    std::unordered_map<chunk_id, std::future<ChunkPrefab>> _chunkPrefabs;
};

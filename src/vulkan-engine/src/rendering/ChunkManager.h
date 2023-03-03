//
// Created by Stanislav Svědiroh on 09.07.2022.
//
#pragma once

#include "VulkanEngineDevice.h"
#include "ChunkDeserializer.h"
#include "Block.h"
#include "Chunk.h"
#include "../threading/BS_thread_pool.h"
#include "../CoordinateSystem.h"
#include "../GlobalConfiguration.h"
#include "../profiling/Timer.h"

#include <unordered_map>
#include "glm/glm.hpp"
#include "fmt/include/fmt/core.h"
#include <cassert>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>

class ChunkManager {
public:
    using ChunkMap = std::unordered_map<Chunk::chunk_id, Chunk>;

    explicit ChunkManager(VulkanEngineDevice &device) : _device{device} {};
    ~ChunkManager() = default;

    ChunkManager(const ChunkManager &) = delete;
    ChunkManager &operator=(const ChunkManager &) = delete;

    Chunk &getChunk(const Chunk::chunk_id &id) { return _chunks[id]; };

    void loadChunksAroundPlayerAsync(glm::vec3 player_pos, uint32_t distance);

    ChunkMap &getVisibleChunks();
private:

    VulkanEngineModel::Builder generateChunkGameObjectPrefab(glm::uvec2 position);

    void populateChunk(Chunk &chunk, ChunkDeserializer::RawChunkData rawData);

    VulkanEngineDevice &_device;
    ChunkDeserializer terrainDeserializer{};

    BS::thread_pool pool{};
    uint32_t max_running_jobs = 10;

    ChunkMap _chunks = {};
};

#include "ChunkManager.h"

void ChunkManager::loadChunksAroundPlayerAsync(glm::vec3 player_pos, uint32_t distance) {
    if (pool.get_tasks_running() > 0) return;
    auto chunk_pos = Chunk::getChunkFromPlayerPos(player_pos);

    // Generate needed chunk positions
    std::vector<glm::vec2> chunk_positions{};
    std::vector<glm::vec2> nodes{};

    // Set the first node at player's position
    chunk_positions.emplace_back(chunk_pos);
    nodes.emplace_back(chunk_pos);

    // Iterate and find nodes up to saved distance
    for (int i = 0; i < distance; i++) {
        std::vector<glm::vec2> nodes_snapshot = nodes;
        for (auto current_pos: nodes_snapshot) {

            // Node to the left
            glm::vec2 l_node = chunkToTheLeft(current_pos);
            if (isInsideMapRange(l_node)) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), l_node) == chunk_positions.end()) {
                    chunk_positions.push_back(l_node);
                    nodes.push_back(l_node);
                }
            }

            // Node to the top
            glm::vec2 t_node = chunkToTheTop(current_pos);
            if (isInsideMapRange(t_node)) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), t_node) == chunk_positions.end()) {
                    chunk_positions.push_back(t_node);
                    nodes.push_back(t_node);
                }
            }

            // Node to the right
            glm::vec2 r_node = chunkToTheRight(current_pos);
            if (isInsideMapRange(r_node)) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), r_node) == chunk_positions.end()) {
                    chunk_positions.push_back(r_node);
                    nodes.push_back(r_node);
                }
            }

            // Node to the bottom
            glm::vec2 b_node = chunkToTheBottom(current_pos);
            if (isInsideMapRange(b_node)) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), b_node) == chunk_positions.end()) {
                    chunk_positions.push_back(b_node);
                    nodes.push_back(b_node);
                }
            }

            // Remove currently already traversed node
            nodes.erase(nodes.begin());
        }
    }

    std::vector<Chunk::chunk_id> deleteIds = {};
    for (auto &chunk: _chunks) {
        if (chunk.second.getChunkState() == CHUNK_STATE_DELETED) {
            deleteIds.push_back(chunk.second.getChunkId());
        }
    }
    for (const auto& deleteId: deleteIds) {
        _chunks.erase(deleteId);
    }

    // Get the desired chunks asynchronously using thread pool
    pool.paused = true;
    uint32_t running_jobs = 0;
    for (auto ch_pos: chunk_positions) {
        if (running_jobs >= max_running_jobs) { return; }
        Chunk::chunk_id id = Chunk::getChunkId(ch_pos);
        if (_chunks.find(id) == _chunks.end()) {
            // Only query those that are not already visible and not in requested state
            _chunks.emplace(id, std::move(Chunk{id, ch_pos}));

            _chunks[id].setChunkPrefabFuture(pool.submit([this](const glm::uvec2 position) { return generateChunkGameObjectPrefab(position); }, ch_pos));
            running_jobs += 1;
        } else {
            // Reactivate those already existing
            _chunks[id].activate();
        }
    }
    pool.paused = false;
}

void ChunkManager::tryToDeleteTimedOutChunks() {

}


VulkanEngineModel::Builder ChunkManager::generateChunkGameObjectPrefab(const glm::uvec2 position) {
    VulkanEngineModel::Builder terrainBuilder{};

    const Chunk::chunk_id &id = Chunk::getChunkId(position);
    fmt::print("Chunk {}_{} begins deserializing\n", position.x, position.y);

    ChunkDeserializer::RawChunkData rawData = terrainDeserializer.deserializeChunkFromDb(position);
    populateChunk(getChunk(id), rawData);

    long i = 0;
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                if (getChunk(id).getBlock({x, y, z}).getBlockId() == Block::BlockTypes::SOLID) {

                    bool leftFace = true;
                    if (x > 0 && getChunk(id).getBlock({x - 1, y, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        leftFace = false;
                    }

                    bool rightFace = true;
                    if (x < CHUNK_SIZE - 1 && getChunk(id).getBlock({x + 1, y, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        rightFace = false;
                    }

                    bool topFace = true;
                    if (z < CHUNK_DEPTH - 1 && getChunk(id).getBlock({x, y, z + 1}).getBlockId() == Block::BlockTypes::SOLID) {
                        topFace = false;
                    }

                    bool bottomFace = true;
                    if (z > 0 && getChunk(id).getBlock({x, y, z - 1}).getBlockId() == Block::BlockTypes::SOLID) {
                        bottomFace = false;
                    }

                    bool frontFace = true;
                    if (y > 0 && getChunk(id).getBlock({x, y - 1, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        frontFace = false;
                    }

                    bool backFace = true;
                    if (y < CHUNK_SIZE - 1 && getChunk(id).getBlock({x, y + 1, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        backFace = false;
                    }

                    VulkanEngineModel::Builder faces = Block::getCubeFaces({x, y, z}, {1.0f, 1.0f, 1.0f}, leftFace, rightFace, topFace, bottomFace, frontFace,
                                                                           backFace);
                    for (auto vertex: faces.vertices) {
                        terrainBuilder.vertices.emplace_back(vertex);
                    }
                    for (auto index: faces.indices) {
                        terrainBuilder.indices.emplace_back(index + i);
                    }
                    i += (long) faces.vertices.size();
                }
            }
        }
    }

    return terrainBuilder;
}

void ChunkManager::populateChunk(Chunk &chunk, ChunkDeserializer::RawChunkData rawData) {
    assert(rawData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH && "Raw chunk data has incorrect size");
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH; i++) {
        if (x == CHUNK_SIZE) {
            x = 0;
            y += 1;
        }
        if (y == CHUNK_SIZE) {
            z += 1;
            y = 0;
        }

        Block &b = chunk.getBlock({x, y, z});
        unsigned char data = rawData[i];
        b.setBlockId(data);
        x += 1;
    }
}

ChunkManager::ChunkMap &ChunkManager::getVisibleChunks() {
    for (auto &chunk: _chunks) {
        chunk_state state = chunk.second.getChunkState();
        if (state == CHUNK_STATE_ACTIVE) {
            // If chunk is in active state, create its game object
            if (chunk.second.checkIfPrefabReady()) {

            }
        }
    }

    return _chunks;
}

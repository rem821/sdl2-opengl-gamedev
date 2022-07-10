#include "ChunkManager.h"


void ChunkManager::loadChunksAroundPlayer(glm::uvec2 player_pos, uint32_t distance) {
    auto chunk_pos = getChunkFromPlayerPos(player_pos);
    // Generate needed chunk positions
    std::vector<glm::vec2> chunk_positions{};
    std::vector<glm::vec2> nodes{};

    // Set the first node at player's position
    chunk_positions.emplace_back(chunk_pos);
    nodes.emplace_back(chunk_pos);

    // Iterate and find nodes up to saved distance
    for (int i = 0; i < distance; i++) {
        int node_index = 0;
        std::vector<glm::vec2> nodes_snapshot = nodes;
        for (auto current_pos: nodes_snapshot) {

            // Node to the left
            glm::vec2 l_node = {current_pos.x + CHUNK_SIZE, current_pos.y};
            if (l_node.x >= 0 && l_node.x < MAP_WIDTH) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), l_node) == chunk_positions.end()) {
                    chunk_positions.push_back(l_node);
                    nodes.push_back(l_node);
                }
            }

            // Node to the top
            glm::vec2 t_node = {current_pos.x, current_pos.y + CHUNK_SIZE};
            if (t_node.y >= 0 && t_node.y < MAP_HEIGHT) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), t_node) == chunk_positions.end()) {
                    chunk_positions.push_back(t_node);
                    nodes.push_back(t_node);
                }
            }

            // Node to the right
            glm::vec2 r_node = {current_pos.x - CHUNK_SIZE, current_pos.y};
            if (r_node.x >= 0 && r_node.x < MAP_WIDTH) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), r_node) == chunk_positions.end()) {
                    chunk_positions.push_back(r_node);
                    nodes.push_back(r_node);
                }
            }

            // Node to the bottom
            glm::vec2 b_node = {current_pos.x, current_pos.y - CHUNK_SIZE};
            if (b_node.y >= 0 && b_node.y < MAP_HEIGHT) {
                if (std::find(chunk_positions.begin(), chunk_positions.end(), b_node) == chunk_positions.end()) {
                    chunk_positions.push_back(b_node);
                    nodes.push_back(b_node);
                }
            }

            // Remove currently already traversed node
            nodes.erase(nodes.begin() + node_index);
            node_index++;
        }
    }

    // Get the desired chunks
    for (auto ch_pos: chunk_positions) {
        generateChunkGameObject(ch_pos);
    }
}


void ChunkManager::generateChunkGameObject(glm::uvec2 position) {
    VulkanEngineModel::Builder terrainBuilder{};

    const chunk_id &id = getChunkId(position);
    if (_activeChunks.find(id) != _activeChunks.end()) { return; }
    if (_chunks.find(id) == _chunks.end()) {
        ChunkDeserializer::RawChunkData rawData = terrainDeserializer.deserializeChunkFromDb(position);
        populateChunk(getChunk(id), rawData);

        fmt::print("Chunk {}_{} deserialized\n", position.x, position.y);
    }

    int i = 0;
    for (int z = 0; z < CHUNK_DEPTH; ++z) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                if (getChunkBlock(id, {x, y, z}).getBlockId() == Block::BlockTypes::SOLID) {

                    bool leftFace = true;
                    if (y > 0 && getChunkBlock(id, {x, y - 1, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        leftFace = false;
                    }

                    bool rightFace = true;
                    if (y < CHUNK_SIZE - 1 && getChunkBlock(id, {x, y + 1, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        rightFace = false;
                    }

                    bool topFace = true;
                    if (z < CHUNK_DEPTH - 1 && getChunkBlock(id, {x, y, z + 1}).getBlockId() == Block::BlockTypes::SOLID) {
                        topFace = false;
                    }

                    bool bottomFace = true;
                    if (z > 0 && getChunkBlock(id, {x, y, z - 1}).getBlockId() == Block::BlockTypes::SOLID) {
                        bottomFace = false;
                    }

                    bool frontFace = true;
                    if (x > 0 && getChunkBlock(id, {x - 1, y, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        frontFace = false;
                    }

                    bool backFace = true;
                    if (x < CHUNK_SIZE - 1 && getChunkBlock(id, {x + 1, y, z}).getBlockId() == Block::BlockTypes::SOLID) {
                        backFace = false;
                    }

                    VulkanEngineModel::Builder faces = Block::getCubeFaces({x, y, -z}, {1.0f, 1.0f, 1.0f}, leftFace, rightFace, topFace, bottomFace, frontFace,
                                                                           backFace);
                    for (auto vertex: faces.vertices) {
                        terrainBuilder.vertices.emplace_back(vertex);
                    }
                    for (auto index: faces.indices) {
                        terrainBuilder.indices.emplace_back(index + i);
                    }
                    i += (int) faces.vertices.size();
                }
            }
        }
    }

    GameObject obj = GameObject::createGameObject(id);
    obj.model = std::make_unique<VulkanEngineModel>(_device, terrainBuilder);
    obj.color = glm::vec3(1.0f, 0.0f, 0.0f);
    obj.transform.translation = {position.y, 0, position.x};

    _activeChunks.emplace(id, std::move(obj));
}

GameObject ChunkManager::getChunkBorders(glm::vec2 chunk_pos) {
    VulkanEngineModel::Builder bordersBuilder{};

    glm::vec3 size = {CHUNK_SIZE, CHUNK_DEPTH, CHUNK_SIZE};
    glm::vec3 pos = {(CHUNK_SIZE / 2.0f) - 0.5f, (CHUNK_SIZE / 2.0f) - 0.5f, -CHUNK_DEPTH / 2};

    VulkanEngineModel::Builder faces = Block::getCubeFaces(pos, size, true, true, false, false, true, true);

    for (auto vertex: faces.vertices) {
        bordersBuilder.vertices.emplace_back(vertex);
    }
    for (auto index: faces.indices) {
        bordersBuilder.indices.emplace_back(index);
    }

    GameObject obj = GameObject::createGameObject();
    obj.model = std::make_unique<VulkanEngineModel>(_device, bordersBuilder);
    obj.color = glm::vec3(1.0f, 1.0f, 0.0f);
    obj.transform.translation = {chunk_pos.y, 0, chunk_pos.x};
    obj.isWireFrame = true;
    obj.isActive = false;

    return obj;
}

void ChunkManager::populateChunk(ChunkManager::Chunk &chunk, ChunkDeserializer::RawChunkData rawData) {
    assert(rawData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH && "Raw chunk data has incorrect size");
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;

    for (uint32_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH; i++) {
        if (x == CHUNK_SIZE) {
            x = 0;
            y += 1;
        }
        if (y == CHUNK_SIZE) {
            z += 1;
            y = 0;
        }

        getChunkBlock(chunk, {x, y, z}).setBlockId(rawData[i]);
        x += 1;
    }
}

glm::uvec2 ChunkManager::getChunkFromPlayerPos(glm::uvec2 player_pos) {
    if (player_pos.x <= 0 && player_pos.y <= 0) {
        return {0, 0};
    } else if (player_pos.x <= 0) {
        return {0, std::max<uint32_t>((uint32_t) (player_pos.y / CHUNK_SIZE) * CHUNK_SIZE, 0)};
    } else if ( player_pos.y <= 0 )
    {
        return {std::max<uint32_t>((uint32_t) (player_pos.x / CHUNK_SIZE) * CHUNK_SIZE, 0), 0};
    } else {
        return {std::max<uint32_t>((uint32_t) (player_pos.x / CHUNK_SIZE) * CHUNK_SIZE, 0),
                std::max<uint32_t>((uint32_t) (player_pos.y / CHUNK_SIZE) * CHUNK_SIZE, 0)
        };
    }

}

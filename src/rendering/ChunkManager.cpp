#include "ChunkManager.h"


GameObject ChunkManager::getChunkGameObject(glm::uvec2 position) {
    VulkanEngineModel::Builder terrainBuilder{};

    const chunk_id &id = getChunkId(position);
    if (!isChunkLoaded[id]) {
        ChunkDeserializer::RawChunkData rawData = terrainDeserializer.deserializeChunk(position);
        populateChunk(getChunk(id), rawData);
        fmt::print("Chunk {}_{} deserialized\n", position.x, position.y);
    }

    Chunk &ch = getChunk(id);

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

    GameObject obj = GameObject::createGameObject();
    obj.model = std::make_unique<VulkanEngineModel>(_device, terrainBuilder);
    obj.color = glm::vec3(1.0f, 0.0f, 0.0f);
    obj.transform.translation = {position.y, 0, position.x};

    return obj;
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

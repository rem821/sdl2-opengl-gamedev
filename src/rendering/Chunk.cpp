#include "Chunk.h"

GameObject Chunk::getChunkBorders(VulkanEngineDevice &_device, glm::vec2 chunk_pos) {
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

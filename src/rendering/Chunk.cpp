#include "Chunk.h"

GameObject Chunk::getChunkBorders(VulkanEngineDevice &_device, const Chunk &chunk) {
    VulkanEngineModel::Builder bordersBuilder{};

    glm::vec3 size = {CHUNK_SIZE, CHUNK_SIZE, CHUNK_DEPTH};
    glm::vec3 pos = {chunk._position, 0};

    VulkanEngineModel::Builder faces = Block::getCubeFaces(pos, size, {1.f, 1.f, 0.f}, true, true, false, false, true, true);

    for (auto vertex: faces.vertices) {
        bordersBuilder.vertices.emplace_back(vertex);
    }
    for (auto index: faces.indices) {
        bordersBuilder.indices.emplace_back(index);
    }

    GameObject obj = GameObject::createGameObject(fmt::format("{}-border", chunk._gameObjectId));
    obj.model = std::make_unique<VulkanEngineModel>(_device, bordersBuilder);
    obj.color = glm::vec3(1.0f, 0.0f, 0.0f);
    obj.transform.translation = {0.0f, 0.0f, 0.0f};
    obj.renderMode = GameObject::RENDER_MODE_WIREFRAME;
    obj.isActive = false;

    return obj;
}

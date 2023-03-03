//
// Created by Stanislav Svědiroh on 12.07.2022.
//
#pragma once

#include "Block.h"
#include "GameObject.h"
#include "VulkanEngineModel.h"
#include "../CoordinateSystem.h"
#include "../GlobalConfiguration.h"


#include "glm/glm.hpp"
#include "fmt/include/fmt/core.h"
#include <chrono>
#include <future>
#include <vector>
#include <cassert>

typedef enum {
    CHUNK_STATE_INVALID,
    CHUNK_STATE_REQUESTED,
    CHUNK_STATE_ACTIVE,
    CHUNK_STATE_VISIBLE,
    CHUNK_STATE_INVALIDATED,
} chunk_state;

class Chunk {
public:
    using chunk_id = std::string;
    using chunk_prefab = std::future<VulkanEngineModel::Builder>;

    explicit Chunk(chunk_id id = "error", glm::uvec2 pos = {0, 0}) : _id(std::move(id)), _position(pos), _state(CHUNK_STATE_REQUESTED) {
        activate();
    };

    ~Chunk() = default;

    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;
    Chunk(Chunk &&) = default;
    Chunk &operator=(Chunk &&) = default;

    chunk_id getChunkId() {
        return _id;
    };

    glm::uvec2 getChunkPosition() { return _position; };

    bool isTimedOut() {
        return std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::steady_clock::now() - _activationTime).count() > CHUNK_LIFESPAN_SECONDS;
    }

    void activate() {
        _activationTime = std::chrono::steady_clock::now();
    }

    Block &getBlock(glm::uvec3 pos) { return _blocks[fromWorldToChunkSerial(pos)]; }

    void setBlockId(glm::uvec3 pos, Block::block_id id) { getBlock(pos).setBlockId(id); }

    static chunk_id getChunkId(glm::uvec2 position) { return fmt::format("{}_{}", position.x, position.y); };

    static glm::uvec2 getChunkFromPlayerPos(glm::vec3 player_pos) {
        if (!isInsideMapRange(player_pos)) { return {0, 0}; }
        return {std::max<uint32_t>((uint32_t) ((player_pos.x + 0.001f) / CHUNK_SIZE) * CHUNK_SIZE, 0),
                std::max<uint32_t>((uint32_t) ((player_pos.y + 0.001f) / CHUNK_SIZE) * CHUNK_SIZE, 0)
        };
    }

    void setChunkPrefabFuture(chunk_prefab prefab) {
        assert (_state == CHUNK_STATE_REQUESTED && "Chunk is not in requested state, did you initialize it properly?");
        _chunkPrefabFuture = std::move(prefab);
        _state = CHUNK_STATE_ACTIVE;
    };

    bool checkIfPrefabReady() {
        assert (_state == CHUNK_STATE_ACTIVE && "Chunk is not in active state, cannot check Prefab readiness!");
        return _chunkPrefabFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    GameObject createGameObject(VulkanEngineDevice &device) {
        assert (_state == CHUNK_STATE_ACTIVE && "Chunk is not in active state, cannot create GameObject!");
        assert (checkIfPrefabReady() && "Chunk prefab future is not ready yet, check before calling this function!");

        VulkanEngineModel::Builder result = _chunkPrefabFuture.get();

        GameObject obj = GameObject::createGameObject(_id);
        obj.model = std::make_unique<VulkanEngineModel>(device, result);
        obj.color = glm::vec3(1.0f, 0.0f, 0.0f);
        obj.transform.translation = {_position.y, 0, _position.x};

        _gameObjectId = obj.getId();
        _state = CHUNK_STATE_VISIBLE;

        //fmt::print("Chunk {}_{} created\n", _position.x, _position.y);

        return obj;
    };

    chunk_state getChunkState() { return _state; }

    id_t getGameObjectId() { return _gameObjectId; }

    id_t getBorderGameObjectId() { return GameObject::generateGameObjectId(fmt::format("{}-border", _gameObjectId)); }

    void invalidate() { _state = CHUNK_STATE_INVALIDATED; }

    static GameObject getChunkBorders(VulkanEngineDevice &_device, const Chunk &chunk);

private:

    chunk_id _id;
    chunk_state _state;
    glm::uvec2 _position;
    std::chrono::time_point<std::chrono::steady_clock> _activationTime;
    chunk_prefab _chunkPrefabFuture;
    id_t _gameObjectId;

    Block _blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH];

};
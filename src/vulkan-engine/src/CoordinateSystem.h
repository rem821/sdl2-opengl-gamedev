//
// Created by Stanislav Svědiroh on 12.07.2022.
//
#pragma once

#include "GlobalConfiguration.h"

#include "glm/glm.hpp"

// World coordinate system - Left handed

static glm::vec3 fromCameraToWorld(glm::vec3 pos) {
    return {pos.z, pos.x, -pos.y};
}

static glm::vec3 fromWorldToCamera(glm::vec3 pos) {
    return {pos.y, -pos.z, pos.x};
}



// Chunk operations - works with the world coordinates only!!

static uint32_t fromWorldToChunkSerial(glm::uvec3 pos) {
    return pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE;
}

static glm::vec2 chunkToTheLeft(glm::vec2 pos) {
    return {pos.x + CHUNK_SIZE, pos.y};
}

static glm::vec2 chunkToTheRight(glm::vec2 pos) {
    return {pos.x - CHUNK_SIZE, pos.y};
}

static glm::vec2 chunkToTheTop(glm::vec2 pos) {
    return {pos.x, pos.y + CHUNK_SIZE};
}

static glm::vec2 chunkToTheBottom(glm::vec2 pos) {
    return {pos.x, pos.y - CHUNK_SIZE};
}

// Map operations - works with the world coordinates only!!

static bool isInsideMapRange(glm::vec2 pos) {
    return pos.x >= 0 && pos.x < MAP_WIDTH && pos.y >= 0 && pos.y < MAP_HEIGHT;
}

static bool isInsideMapRange(glm::vec3 pos) {
    return isInsideMapRange({pos.x, pos.y});
}


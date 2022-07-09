#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"
#include "Block.h"

#include <glm/glm.hpp>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define MAP_HEIGHT 544
#define MAP_WIDTH 960

class ChunkDeserializer {
public:
    using RawChunkData = std::vector<unsigned char>;

    ChunkDeserializer() = default;
    ~ChunkDeserializer() = default;

    RawChunkData deserializeChunk(glm::uvec2 chunk_pos);

private:
};
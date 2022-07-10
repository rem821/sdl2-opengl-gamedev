#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"
#include "Block.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <glm/glm.hpp>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define MAP_HEIGHT 1088
#define MAP_WIDTH 1920

class ChunkDeserializer {
public:
    using RawChunkData = std::vector<unsigned char>;

    ChunkDeserializer() : db(SQLite::Database("assets/map/mars.db3", SQLite::OPEN_READONLY)) {};
    ~ChunkDeserializer() = default;

    RawChunkData deserializeChunkFromFile(glm::uvec2 chunk_pos);
    std::string readSerialChunkFromFile(glm::uvec2 chunk_pos);

    RawChunkData deserializeChunkFromDb(glm::uvec2 chunk_pos);


    [[maybe_unused]] void createDatabaseFile();

private:

    SQLite::Database db;
};
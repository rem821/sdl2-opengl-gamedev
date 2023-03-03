#pragma once

#include "VulkanEngineDevice.h"
#include "GameObject.h"
#include "Block.h"
#include "../GlobalConfiguration.h"

#include "SQLiteCpp/SQLiteCpp.h"
#include "glm/glm.hpp"
#include "fmt/include/fmt/core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class ChunkDeserializer {
public:
    using RawChunkData = std::vector<unsigned char>;

    ChunkDeserializer() : db(SQLite::Database("assets/map/mars.db3", SQLite::OPEN_READONLY)) {};
    ~ChunkDeserializer() = default;

    RawChunkData deserializeChunkFromFile(glm::uvec2 chunk_pos);
    std::string readSerialChunkFromFile(glm::uvec2 chunk_pos);

    RawChunkData deserializeChunkFromDb(glm::uvec2 chunk_pos);


    void createDatabaseFile();

private:

    SQLite::Database db;
};
#include "ChunkDeserializer.h"

ChunkDeserializer::RawChunkData ChunkDeserializer::deserializeChunk(glm::uvec2 chunk_pos) {
    std::ifstream infile(fmt::format("assets/map/{}_{}_level.txt", chunk_pos.x, chunk_pos.y));

    RawChunkData chunkData{};

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        std::string buff("");

        for (unsigned char ch: line) {
            if (ch == Block::BlockTypes::SOLID || ch == Block::BlockTypes::AIR) {
                // If we hit block type, fetch the previously saved number
                int length;
                std::stringstream str(buff);
                str >> length;

                for (uint32_t i = 0; i < length; i++) {
                    chunkData.push_back(ch);
                }

                buff = "";
            } else {
                // Put a number aside
                buff.push_back(ch);
            }
        }
    }
    infile.close();
    return chunkData;
}
#include "../../include/rendering/ChunkDeserializer.h"

ChunkDeserializer::RawChunkData ChunkDeserializer::deserializeChunkFromFile(glm::uvec2 chunk_pos) {
    std::ifstream infile(fmt::format("assets/map/{}_{}_level.txt", chunk_pos.x, chunk_pos.y));

    RawChunkData chunkData{};

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        std::string buff;

        for (char ch: line) {
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

std::string ChunkDeserializer::readSerialChunkFromFile(glm::uvec2 chunk_pos) {
    std::ifstream infile(fmt::format("assets/map/{}_{}_level.txt", chunk_pos.x, chunk_pos.y));

    std::string line;
    std::string buff;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        for (char ch: line) {
            buff.push_back(ch);
        }
    }
    infile.close();
    return buff;
}

ChunkDeserializer::RawChunkData ChunkDeserializer::deserializeChunkFromDb(glm::uvec2 chunk_pos) {
    RawChunkData chunkData{};

    std::string id = fmt::format("{}_{}", chunk_pos.x, chunk_pos.y);

    SQLite::Statement query(db, "SELECT serialized FROM chunks WHERE id = ?");
    query.bind(1, id);

    std::string serialized;
    while (query.executeStep()) {
        serialized = query.getColumn(0).getString();
    }

    std::string buff;
    uint32_t i = 0;
    while (i < serialized.size()) {
        char ch = serialized[i];

        if (ch == Block::BlockTypes::SOLID || ch == Block::BlockTypes::AIR) {
            // If we hit block type, fetch the previously saved number
            int length;
            std::stringstream str(buff);
            str >> length;

            for (uint32_t j = 0; j < length; j++) {
                chunkData.push_back(ch);
            }

            buff = "";
        } else {
            // Put a number aside
            buff.push_back(ch);
        }

        i++;
    }

    return chunkData;
}

void ChunkDeserializer::createDatabaseFile() {
    SQLite::Database _db("assets/map/mars.db3", SQLite::OPEN_READWRITE);

    SQLite::Statement createTable(_db, "");

    SQLite::Transaction transaction(_db);
    _db.exec("CREATE TABLE chunks (id TEXT PRIMARY KEY, serialized TEXT)");
    transaction.commit();

    int chunks_y = MAP_WIDTH / CHUNK_SIZE;
    int chunks_x = MAP_HEIGHT / CHUNK_SIZE;

    for (int j = 0; j < chunks_x; j++) {
        for (int i = 0; i < chunks_y; i++) {
            std::string id = fmt::format("{}_{}", j * CHUNK_SIZE, i * CHUNK_SIZE);

            std::string data = readSerialChunkFromFile({j * CHUNK_SIZE, i * CHUNK_SIZE});

            SQLite::Transaction tr(_db);
            _db.exec(fmt::format("INSERT INTO chunks (id, serialized) VALUES ('{}','{}')", id, data));
            tr.commit();

        }
    }

}
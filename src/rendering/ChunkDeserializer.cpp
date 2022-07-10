#include "ChunkDeserializer.h"

ChunkDeserializer::RawChunkData ChunkDeserializer::deserializeChunkFromFile(glm::uvec2 chunk_pos) {
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

    SQLite::Database db("assets/map/mars.db3", SQLite::OPEN_READONLY);
    std::string id = fmt::format("{}_{}", chunk_pos.x , chunk_pos.y);

    SQLite::Statement query(db, "SELECT serialized FROM chunks WHERE id = ?");
    query.bind(1, id);

    std::string serialized = "";
    while (query.executeStep())
    {
        serialized = query.getColumn(0).getString();
    }

    std::string buff("");
    uint32_t i = 0;
    while (i < serialized.size()) {
        unsigned char ch = serialized[i];

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
    SQLite::Database db("assets/map/mars.db3", SQLite::OPEN_READWRITE);

    SQLite::Statement createTable(db, "");

    SQLite::Transaction transaction(db);
    db.exec("CREATE TABLE chunks (id TEXT PRIMARY KEY, serialized TEXT)");
    transaction.commit();

    int CHUNK_SIZE = 32;
    int CHUNK_DEPTH = 256;
    int WORLD_WIDTH = 1920;
    int WORLD_HEIGHT = 1088;

    int chunks_x = WORLD_WIDTH / CHUNK_SIZE;
    int chunks_y = WORLD_HEIGHT / CHUNK_SIZE;

    for (int j = 0; j < chunks_x; j++) {
        for (int i = 0; i < chunks_y; i++) {
            std::string id = fmt::format("{}_{}", j * CHUNK_SIZE, i * CHUNK_SIZE);

            std::string data = readSerialChunkFromFile({j * CHUNK_SIZE, i * CHUNK_SIZE});

            SQLite::Transaction tr(db);
            db.exec(fmt::format("INSERT INTO chunks (id, serialized) VALUES ('{}','{}')", id, data));
            tr.commit();

        }
    }

}
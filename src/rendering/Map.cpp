#include "Map.h"

Map::Map(VulkanEngineDevice &engineDevice, Cube &cube) : device{engineDevice}, _cube{cube} {
    for (int z = 0; z < MAP_DEPTH; ++z) {
        loadLevel(z);
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                map_blocks[z][x][y] = level[z][x][y];
            }
        }
    }
}

Map::~Map() {}

void Map::loadLevel(int z) {
    std::ifstream infile(fmt::format("assets/map/level_{}.txt", z));

    int x = 0;
    int y = 0;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        for (int i = 0; i < line.size(); ++i) {
            char ch = line[i];

            if (ch == '1' || ch == '0') {
                level[z][x][y] = ch - '0';
                x += 1;
            }
        }
        y += 1;
        x = 0;
    }

    infile.close();
    fmt::print(fmt::format("Level {} processed\n", z));
}

GameObject Map::getMapBlocks() {
    VulkanEngineModel::Builder terrainBuilder{};

    int i = 0;
    for (int z = 0; z < MAP_DEPTH; ++z) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                if (map_blocks[z][x][y] == 1) {

                    bool leftFace = true;
                    if (y > 0 && map_blocks[z][x][y - 1] == 1) {
                        leftFace = false;
                    }

                    bool rightFace = true;
                    if (y < MAP_HEIGHT - 1 && map_blocks[z][x][y + 1] == 1) {
                        rightFace = false;
                    }

                    bool topFace = true;
                    if (z < MAP_DEPTH - 1 && map_blocks[z + 1][x][y] == 1) {
                        topFace = false;
                    }

                    bool bottomFace = true;
                    if (z > 0 && map_blocks[z - 1][x][y] == 1) {
                        bottomFace = false;
                    }

                    bool frontFace = true;
                    if (x > 0 && map_blocks[z][x - 1][y] == 1) {
                        frontFace = false;
                    }

                    bool backFace = true;
                    if (x < MAP_WIDTH - 1 && map_blocks[z][x + 1][y] == 1) {
                        backFace = false;
                    }

                    VulkanEngineModel::Builder faces = _cube.getCubeFaces(x, y, -z, leftFace, rightFace, topFace, bottomFace, frontFace, backFace);
                    for (auto vertex: faces.vertices) {
                        terrainBuilder.vertices.emplace_back(vertex);
                    }
                    for (auto index: faces.indices) {
                        terrainBuilder.indices.emplace_back(index + i);
                    }
                    i += (int) faces.vertices.size();
                }
            }
        }
    }

    GameObject obj = GameObject::createGameObject();
    obj.model = std::make_unique<VulkanEngineModel>(device, terrainBuilder);
    obj.color = glm::vec3(1.0f, 0.0f, 0.0f);

    return obj;
}

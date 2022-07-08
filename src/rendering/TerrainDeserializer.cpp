#include "TerrainDeserializer.h"

TerrainDeserializer::TerrainDeserializer(VulkanEngineDevice &engineDevice) : device{engineDevice} {
    for (int i = 0; i < MAP_HEIGHT / CHUNK_SIZE; i++) {
        for (int j = 0; j < MAP_WIDTH / CHUNK_SIZE; j++) {
            int x = j * CHUNK_SIZE;
            int y = i * CHUNK_SIZE;
            deserializeChunk({x, y});
        }
    }
}

TerrainDeserializer::~TerrainDeserializer() = default;

TerrainDeserializer::Block &TerrainDeserializer::getChunkBlock(const chunk_id &id, glm::uvec3 pos) {
    return chunks[id][pos.x + pos.z * CHUNK_SIZE + pos.y * CHUNK_SIZE * MAP_DEPTH];
}

void TerrainDeserializer::deserializeChunk(glm::uvec2 chunk_pos) {
    std::ifstream infile(fmt::format("assets/map/{}_{}_level.txt", chunk_pos.x, chunk_pos.y));

    uint x = 0;
    uint y = 0;
    uint z = 0;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        std::string buff("");

        for (char ch: line) {
            if (ch == 'a' || ch == 's') {
                // If we hit block type, fetch the previously saved number
                int length;
                std::stringstream str(buff);
                str >> length;

                Block block;
                if (ch == 's') { block = 1; } else { block = 0; }

                for (uint i = 0; i < length; i++) {
                    getChunkBlock(getChunkId(chunk_pos), {x + i, y, z}) = block;
                }

                x += length;
                if (x > CHUNK_SIZE) {
                    fmt::print("s");
                }
                assert(x <= CHUNK_SIZE && "Chunk width too large");
                if (x == CHUNK_SIZE) {
                    x = 0;
                    y += 1;
                }

                assert(y <= CHUNK_SIZE && "Chunk height too large");
                if (y == CHUNK_SIZE) {
                    z += 1;
                    y = 0;
                }

                assert(z <= MAP_DEPTH && "Chunk depth too large");
                if (z == MAP_DEPTH) {
                    fmt::print("Level processed\n");
                    infile.close();
                    return;
                }

                buff = "";
            } else {
                // Put a number aside
                buff.push_back(ch);
            }
        }
    }

}

GameObject TerrainDeserializer::getChunkGameObject(glm::uvec2 position) {
    VulkanEngineModel::Builder terrainBuilder{};

    const chunk_id& id = getChunkId(position);
    int i = 0;
    for (int z = 0; z < MAP_DEPTH; ++z) {
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                if (getChunkBlock(id, {x, y, z}) == 1) {

                    bool leftFace = true;
                    if (y > 0 && getChunkBlock(id, {x, y - 1, z}) == 1) {
                        leftFace = false;
                    }

                    bool rightFace = true;
                    if (y < MAP_HEIGHT - 1 && getChunkBlock(id, {x, y + 1, z}) == 1) {
                        rightFace = false;
                    }

                    bool topFace = true;
                    if (z < MAP_DEPTH - 1 && getChunkBlock(id, {x, y, z + 1}) == 1) {
                        topFace = false;
                    }

                    bool bottomFace = true;
                    if (z > 0 && getChunkBlock(id, {x, y, z - 1}) == 1) {
                        bottomFace = false;
                    }

                    bool frontFace = true;
                    if (x > 0 && getChunkBlock(id, {x - 1, y, z}) == 1) {
                        frontFace = false;
                    }

                    bool backFace = true;
                    if (x < MAP_WIDTH - 1 && getChunkBlock(id, {x + 1, y, z}) == 1) {
                        backFace = false;
                    }

                    VulkanEngineModel::Builder faces = getCubeFaces({x, y, -z}, {1.0f, 1.0f, 1.0f}, leftFace, rightFace, topFace, bottomFace, frontFace,
                                                                    backFace);
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

VulkanEngineModel::Builder TerrainDeserializer::getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation, glm::vec3 size) {
    VulkanEngineModel::Builder builder = VulkanEngineModel::Builder{};

    if (orientation == TOP || orientation == BOTTOM) {
        int normal;
        if (orientation == TOP) {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = -1.0f;
        } else {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {-(size.x * 0.5f) + x_tr, 0.0f + y_tr, -(size.z * 0.5f) + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, normal, 0.0f};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-(size.x * 0.5f) + x_tr, 0.0f + y_tr, (size.z * 0.5f) + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, normal, 0.0f};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {(size.x * 0.5f) + x_tr, 0.0f + y_tr, (size.z * 0.5f) + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, normal, 0.0f};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {(size.x * 0.5f) + x_tr, 0.0f + y_tr, -(size.z * 0.5f) + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, normal, 0.0f};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == FRONT || orientation == BACK) {
        int normal;
        if (orientation == FRONT) {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = -1.0f;
        } else {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {-(size.x * 0.5f) + x_tr, (size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {-(size.x * 0.5f) + x_tr, -(size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {(size.x * 0.5f) + x_tr, -(size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {(size.x * 0.5f) + x_tr, (size.y * 0.5f) + y_tr, 0.0f + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    } else if (orientation == LEFT || orientation == RIGHT) {
        int normal;
        if (orientation == LEFT) {
            builder.indices = {0, 1, 2, 2, 3, 0};
            normal = -1.0f;
        } else {
            builder.indices = {0, 2, 1, 2, 0, 3};
            normal = 1.0f;
        }

        auto vert1 = VulkanEngineModel::Vertex{};
        vert1.position = {0.0f + x_tr, (size.y * 0.5f) + y_tr, -(size.z * 0.5f) + z_tr};
        vert1.color = {0.8f, 0.8f, 0.8f};
        vert1.uv = {0.0f, 0.0f};
        vert1.normal = {0.0f, 0.0f, normal};

        auto vert2 = VulkanEngineModel::Vertex{};
        vert2.position = {0.0f + x_tr, -(size.y * 0.5f) + y_tr, -(size.z * 0.5f) + z_tr};
        vert2.color = {0.8f, 0.8f, 0.8f};
        vert2.uv = {1.0f, 0.0f};
        vert2.normal = {0.0f, 0.0f, normal};

        auto vert3 = VulkanEngineModel::Vertex{};
        vert3.position = {0.0f + x_tr, -(size.y * 0.5f) + y_tr, (size.z * 0.5f) + z_tr};
        vert3.color = {0.8f, 0.8f, 0.8f};
        vert3.uv = {1.0f, 1.0f};
        vert3.normal = {0.0f, 0.0f, normal};

        auto vert4 = VulkanEngineModel::Vertex{};
        vert4.position = {0.0f + x_tr, (size.y * 0.5f) + y_tr, (size.z * 0.5f) + z_tr};
        vert4.color = {0.8f, 0.8f, 0.8f};
        vert4.uv = {0.0f, 1.0f};
        vert4.normal = {0.0f, 0.0f, normal};

        builder.vertices = {vert1, vert2, vert3, vert4};
    }

    return builder;
}


VulkanEngineModel::Builder
TerrainDeserializer::getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left, bool right, bool top, bool bottom, bool front, bool back) {
    VulkanEngineModel::Builder cubeFaces;

    int i = 0;
    if (left) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y - (0.5f * size.x), world_pos.z, world_pos.x, FaceOrientation::LEFT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }

        i += 4;
    }
    if (right) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y + (0.5f * size.x), world_pos.z, world_pos.x, FaceOrientation::RIGHT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (top) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z - (0.5f * size.y), world_pos.x, FaceOrientation::TOP, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (bottom) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z + (0.5f * size.y), world_pos.x, FaceOrientation::BOTTOM, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    if (front) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z, world_pos.x - (0.5f * size.z), FaceOrientation::FRONT, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }
    if (back) {
        VulkanEngineModel::Builder vertices = getFaceVertices(world_pos.y, world_pos.z, world_pos.x + (0.5f * size.z), FaceOrientation::BACK, size);
        for (auto vertex: vertices.vertices) {
            cubeFaces.vertices.emplace_back(vertex);
        }
        for (auto index: vertices.indices) {
            cubeFaces.indices.emplace_back(index + i);
        }
        i += 4;
    }

    return cubeFaces;
}


GameObject TerrainDeserializer::getChunkBorders() {
    VulkanEngineModel::Builder bordersBuilder{};

    glm::vec3 size = {CHUNK_SIZE, MAP_DEPTH, CHUNK_SIZE};
    uint32_t chunks_x = MAP_WIDTH / CHUNK_SIZE;
    uint32_t chunks_y = MAP_HEIGHT / CHUNK_SIZE;

    int k = 0;
    for (int i = 0; i < chunks_x; i++) {
        for (int j = 0; j < chunks_y; j++) {
            auto x_pos = float(i * CHUNK_SIZE);
            auto y_pos = float(j * CHUNK_SIZE);

            glm::vec3 pos = {x_pos + (CHUNK_SIZE / 2.0f) - 0.5f, y_pos + (CHUNK_SIZE / 2.0f) - 0.5f, -MAP_DEPTH / 2};
            VulkanEngineModel::Builder faces = getCubeFaces(pos, size, true, true, false, false, true, true);

            for (auto vertex: faces.vertices) {
                bordersBuilder.vertices.emplace_back(vertex);
            }
            for (auto index: faces.indices) {
                bordersBuilder.indices.emplace_back(index + k);
            }
            k += (int) faces.vertices.size();
        }
    }

    GameObject obj = GameObject::createGameObject();
    obj.model = std::make_unique<VulkanEngineModel>(device, bordersBuilder);
    obj.color = glm::vec3(1.0f, 1.0f, 0.0f);
    obj.isWireFrame = true;

    return obj;
}
//
// Created by Stanislav Svědiroh on 09.07.2022.
//
#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>
#include <fmt/core.h>

class Block {
public:
    using block_id = unsigned char;
    enum BlockTypes {
        SOLID = 's',
        AIR = 'a'
    };
    enum FaceOrientation {
        LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK
    };


    Block() { _id = BlockTypes::AIR; };

    Block(block_id id) { _id = id; };
    ~Block() = default;

    Block(const Block &) = delete;
    Block &operator=(const Block &) = delete;


    block_id getBlockId() { return _id; };
    void setBlockId(block_id id) { _id = id; };

    static VulkanEngineModel::Builder getFaceVertices(float x_tr, float y_tr, float z_tr, FaceOrientation orientation, glm::vec3 size);
    static VulkanEngineModel::Builder
    getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);

private:

    block_id _id;
};
//
// Created by Stanislav Svědiroh on 09.07.2022.
//
#pragma once

#include "GameObject.h"
#include "../CoordinateSystem.h"
#include "../GlobalConfiguration.h"

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

    explicit Block(block_id &id) { _id = id; };
    ~Block() = default;

    [[nodiscard]] block_id getBlockId() const { return _id; };
    void setBlockId(block_id id) { _id = id; };

    static VulkanEngineModel::Builder getFaceVertices(glm::vec3 pos, FaceOrientation orientation, glm::vec3 size);
    static VulkanEngineModel::Builder
    getCubeFaces(glm::vec3 world_pos, glm::vec3 size, bool left = true, bool right = true, bool top = true, bool bottom = true, bool front = true,
                 bool back = true);

private:

    block_id _id;
};
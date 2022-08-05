//
// Created by Stanislav Svědiroh on 05.08.2022.
//
#pragma once

#include <fmt/core.h>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Utils {

    glm::vec3 getRandomColor() {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        return {r, g, b};
    }
}
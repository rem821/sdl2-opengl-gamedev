//
// Created by Stanislav Svědiroh on 26.06.2022.
//
#include "Cube.h"

Cube::Cube(VulkanEngineDevice &device) : engineDevice{device} {

}


GameObject::Map Cube::getCubeFaces(float world_x, float world_y, float world_z, bool left, bool right, bool top, bool bottom, bool front, bool back) {
    GameObject::Map cubeFaces;

    if (left) {
        auto leftFace = GameObject::createGameObject();
        leftFace.model = quad;
        leftFace.transform.rotation = {.0f, .0f, 3 * glm::half_pi<float>()};
        leftFace.transform.translation = {(world_y * 2.0f) + -1.0f, (world_z * 2.0f), (world_x * 2.0f)};
        leftFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(leftFace.getId(), std::move(leftFace));
    }
    if (right) {
        auto rightFace = GameObject::createGameObject();
        rightFace.model = quad;
        rightFace.transform.rotation = {.0f, .0f, glm::half_pi<float>()};
        rightFace.transform.translation = {(world_y * 2.0f) + 1.0f, (world_z * 2.0f), (world_x * 2.0f)};
        rightFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(rightFace.getId(), std::move(rightFace));
    }

    if (top) {
        auto topFace = GameObject::createGameObject();
        topFace.model = quad;
        topFace.transform.rotation = {.0f, .0f, .0f};
        topFace.transform.translation = {(world_y * 2.0f), (world_z * 2.0f) + -1.0f, (world_x * 2.0f)};
        topFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(topFace.getId(), std::move(topFace));
    }
    if (bottom) {
        auto bottomFace = GameObject::createGameObject();
        bottomFace.model = quad;
        bottomFace.transform.rotation = {.0f, .0f, glm::pi<float>()};
        bottomFace.transform.translation = {(world_y * 2.0f), (world_z * 2.0f) + 1.0f, (world_x * 2.0f)};
        bottomFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(bottomFace.getId(), std::move(bottomFace));
    }

    if (front) {
        auto frontFace = GameObject::createGameObject();
        frontFace.model = quad;
        frontFace.transform.rotation = {glm::half_pi<float>(), .0f, .0f};
        frontFace.transform.translation = {(world_y * 2.0f), (world_z * 2.0f), (world_x * 2.0f) + -1.0f};
        frontFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(frontFace.getId(), std::move(frontFace));
    }
    if (back) {
        auto backFace = GameObject::createGameObject();
        backFace.model = quad;
        backFace.transform.rotation = {3 * glm::half_pi<float>(), .0f, .0f};
        backFace.transform.translation = {(world_y * 2.0f), (world_z * 2.0f), (world_x * 2.0f) + 1.0f};
        backFace.transform.scale = {1.f, 1.f, 1.f};

        cubeFaces.emplace(backFace.getId(), std::move(backFace));
    }

    return cubeFaces;
}

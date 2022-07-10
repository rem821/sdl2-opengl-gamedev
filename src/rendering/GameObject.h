#pragma once

#include "VulkanEngineModel.h"
#include <memory>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4();

    glm::mat3 normalMatrix();
};

struct PointLightComponent {
    float lightIntensity = 1.0f;
};

class GameObject {
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, GameObject>;

    static GameObject createGameObject(const std::string &chunk_id = "") {
        if (chunk_id.empty()) {
            // Used for generic game objects
            static id_t currentId = 0;
            return GameObject{currentId++};
        } else {
            // Used for chunks with predefined chunk_id
            std::hash<std::string> hasher;
            id_t hashed = hasher(chunk_id);
            return GameObject{hashed};
        }
    }

    static GameObject makePointLight(float intensity = 2.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;
    GameObject(GameObject &&) = default;
    GameObject &operator=(GameObject &&) = default;

    id_t getId() { return id; };

    glm::vec3 color{};
    TransformComponent transform{};

    bool isWireFrame = false;
    bool isActive = true;

    // Optional pointer components
    std::shared_ptr<VulkanEngineModel> model{};
    std::unique_ptr<PointLightComponent> pointLight = nullptr;
private:
    GameObject(id_t objId) : id{objId} {};

    id_t id;
};
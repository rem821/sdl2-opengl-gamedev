#pragma once

#include "VulkanEngineModel.h"
#include "VulkanEngineSwapChain.h"
#include <memory>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <fmt/core.h>

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

    enum RenderMode {
        RENDER_MODE_FILLED, RENDER_MODE_WIREFRAME, RENDER_MODE_BOTH
    };

    static GameObject createGameObject(const std::string &chunk_id = "") {
        if (chunk_id.empty()) {
            // Used for generic game objects
            static id_t currentId = 0;
            return GameObject{currentId++};
        } else {
            // Used for chunks with predefined chunk_id
            return GameObject{GameObject::generateGameObjectId(chunk_id)};
        }
    }

    static GameObject makePointLight(float intensity = 2.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

    static id_t generateGameObjectId(const std::string &chunk_id = "") {
        std::hash<std::string> hasher;
        return hasher(chunk_id);
    }

    void invalidate() {
        isInvalidated = true;
        isActive = false;
    }

    bool getIsInvalidated() {
        if(isInvalidated) invalidatedFrames++;
        return isInvalidated;
    }

    bool shouldBeDestroyed() {
        return isInvalidated && (invalidatedFrames > VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    }

    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;
    GameObject(GameObject &&) = default;
    GameObject &operator=(GameObject &&) = default;

    id_t getId() { return id; };

    glm::vec3 color{};
    TransformComponent transform{};

    RenderMode renderMode = RENDER_MODE_FILLED;
    bool isActive = true;

    // Optional pointer components
    std::shared_ptr<VulkanEngineModel> model{};
    std::unique_ptr<PointLightComponent> pointLight = nullptr;
private:
    GameObject(id_t objId) : id{objId} {};

    id_t id;

    uint8_t invalidatedFrames = 0;
    bool isInvalidated = false;
};
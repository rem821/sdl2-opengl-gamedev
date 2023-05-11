#pragma once

#include <precompiled_headers/PCH.h>
#include <platform/vulkan/VulkanModel.h>
#include <platform/vulkan/VulkanSwapChain.h>
#include "glm/gtc/matrix_transform.hpp"

namespace VulkanEngine {
    class VulkanGameObject {
    public:
        struct TransformComponent {
            glm::vec3 translation{};
            glm::vec3 scale{1.f, 1.f, 1.f};
            glm::vec3 rotation{};

            // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
            // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
            // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
            [[nodiscard]] glm::mat4 Mat4() const;

            [[nodiscard]] glm::mat3 NormalMatrix() const;
        };

        struct PointLightComponent {
            float lightIntensity = 1.0f;
        };

        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, VulkanGameObject>;

        enum RenderMode {
            RENDER_MODE_FILLED, RENDER_MODE_WIREFRAME, RENDER_MODE_BOTH
        };

        static VulkanGameObject CreateGameObject(const std::string &chunk_id = "") {
            if (chunk_id.empty()) {
                // Used for generic game objects
                static id_t currentId = 0;
                return VulkanGameObject{currentId++};
            } else {
                // Used for chunks with predefined chunk_id
                return VulkanGameObject{VulkanGameObject::GenerateGameObjectId(chunk_id)};
            }
        }

        static VulkanGameObject MakePointLight(float intensity = 2.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        static id_t GenerateGameObjectId(const std::string &chunk_id = "") {
            std::hash<std::string> hasher;
            return hasher(chunk_id);
        }

        void Invalidate() {
            isInvalidated_ = true;
            isActive = false;
        }

        bool GetIsInvalidated() {
            if(isInvalidated_) invalidatedFrames_++;
            return isInvalidated_;
        }

        [[nodiscard]] bool ShouldBeDestroyed() const {
            return isInvalidated_ && (invalidatedFrames_ > VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        }

        VulkanGameObject(const VulkanGameObject &) = delete;
        VulkanGameObject &operator=(const VulkanGameObject &) = delete;
        VulkanGameObject(VulkanGameObject &&) = default;
        VulkanGameObject &operator=(VulkanGameObject &&) = default;

        [[nodiscard]] id_t GetId() const { return id_; };

        glm::vec3 color{};
        TransformComponent transform{};

        RenderMode renderMode = RENDER_MODE_FILLED;
        bool isActive = true;

        // Optional pointer components
        std::shared_ptr<VulkanModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;
    private:
        explicit VulkanGameObject(id_t objId) : id_{objId} {};

        id_t id_;

        uint8_t invalidatedFrames_ = 0;
        bool isInvalidated_ = false;
    };
}

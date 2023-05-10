//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#pragma once

#include <precompiled_headers/PCH.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"

namespace VulkanEngine {
    class Camera {
    public:

        void SetOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{.0f, -1.f, .0f});

        void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{.0f, -1.f, .0f});

        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

        [[nodiscard]] const glm::mat4 &GetProjection() const { return projectionMatrix_; };

        [[nodiscard]] const glm::mat4 &GetView() const { return viewMatrix_; };

        [[nodiscard]] const glm::mat4 &GetInverseView() const { return inverseViewMatrix_; };

        [[nodiscard]] glm::vec3 GetPosition() const { return position_; };

        [[nodiscard]] glm::vec3 GetRotation() const { return rotation_; };

    private:
        glm::mat4 projectionMatrix_{1.f};
        glm::mat4 viewMatrix_{1.f};
        glm::mat4 inverseViewMatrix_{1.f};

        glm::vec3 position_{1.f};
        glm::vec3 rotation_{1.f};
    };

}

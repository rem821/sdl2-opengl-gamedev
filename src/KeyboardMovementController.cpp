//
// Created by Stanislav Svědiroh on 14.06.2022.
//
#include "KeyboardMovementController.h"

void KeyboardMovementController::moveInPlaneXZ(float dt, GameObject &gameObject) {
    glm::vec3 rotate{0};

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[keys.lookRight]) rotate.y += 1.f;
    if (keystate[keys.lookLeft]) rotate.y -= 1.f;
    if (keystate[keys.lookUp]) rotate.x += 1.f;
    if (keystate[keys.lookDown]) rotate.x -= 1.f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};

    if (keystate[keys.moveForward]) moveDir += forwardDir;
    if (keystate[keys.moveBackward]) moveDir -= forwardDir;
    if (keystate[keys.moveRight]) moveDir += rightDir;
    if (keystate[keys.moveLeft]) moveDir -= rightDir;
    if (keystate[keys.moveUp]) moveDir += upDir;
    if (keystate[keys.moveDown]) moveDir -= upDir;

    float speed = moveSpeed;
    if(keystate[keys.sprint]) speed *= 10;
    if(keystate[keys.sneak]) speed /= 10;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        gameObject.transform.translation += speed * dt * glm::normalize(moveDir);
    }

}

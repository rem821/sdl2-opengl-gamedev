#pragma once

#include "SDL.h"

#include "rendering/VulkanEngineDevice.h"
#include "rendering/VulkanEngineWindow.h"
#include "rendering/VulkanEngineBuffer.h"
#include "rendering/VulkanEngineDescriptors.h"
#include "rendering/VulkanEngineRenderer.h"
#include "rendering/GameObject.h"
#include "rendering/Camera.h"

#include "systems/SimpleRenderSystem.h"
#include "systems/PointLightSystem.h"


#include "KeyboardMovementController.h"

#include "fmt/core.h"

#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <chrono>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Gameska"

struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f}; // w is intensity
    glm::vec3 lightPosition{-1.f};
    alignas(16) glm::vec4 lightColor{1.f}; // w is intensity
};

class Game {

public:
    Game();
    ~Game();

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

private:
    void loadGameObjects();

    void run();
    void handleEvents();

    bool isRunning = false;

    VulkanEngineWindow window{WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};
    VulkanEngineDevice engineDevice{window, WINDOW_TITLE};
    VulkanEngineRenderer renderer{window, engineDevice};

    std::unique_ptr<VulkanEngineDescriptorPool> globalPool{};
    GameObject::Map gameObjects;

    SDL_Rect mouseRect;
};
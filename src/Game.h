#pragma once

#include "SDL.h"

#include "VulkanEngineDevice.h"
#include "VulkanEngineWindow.h"
#include "VulkanEngineBuffer.h"
#include "VulkanEngineDescriptors.h"

#include "SimpleRenderSystem.h"
#include "VulkanEngineRenderer.h"
#include "GameObject.h"
#include "Camera.h"
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
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
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
    std::vector<GameObject> gameObjects;

    SDL_Rect mouseRect;
};
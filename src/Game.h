#pragma once

#include "SDL.h"

#include "rendering/VulkanEngineDevice.h"
#include "rendering/VulkanEngineWindow.h"
#include "rendering/VulkanEngineBuffer.h"
#include "rendering/VulkanEngineDescriptors.h"
#include "rendering/VulkanEngineRenderer.h"
#include "rendering/GameObject.h"
#include "rendering/Camera.h"
#include "rendering/ChunkDeserializer.h"
#include "rendering/ChunkManager.h"
#include "rendering/gui/DebugGui.h"

#include "systems/SimpleRenderSystem.h"
#include "systems/PointLightSystem.h"

#include "KeyboardMovementController.h"
#include "CoordinateSystem.h"

#include <fmt/core.h>

#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <chrono>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Gameska"

class Game {

public:
    Game();
    ~Game() = default;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

private:
    void loadGameObjects();
    void loadChunkGameObjects(glm::vec3 playerPos);

    void run();
    void handleEvents();

    bool isRunning = false;

    VulkanEngineWindow window{WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};
    VulkanEngineDevice engineDevice{window, WINDOW_TITLE};
    VulkanEngineRenderer renderer{window, engineDevice};

    std::unique_ptr<VulkanEngineDescriptorPool> globalPool{};

    ChunkManager chunkManager{engineDevice};
    DebugGui debugGui{engineDevice, renderer, window.sdlWindow()};

    GameObject::Map gameObjects;
    std::vector<uint32_t> chunkBorderIds;

    SDL_Rect mouseRect{};
};
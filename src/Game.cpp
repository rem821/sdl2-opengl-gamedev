#include "Game.h"

Game::Game() {
    loadGameObjects();

    isRunning = true;
    run();
}

Game::~Game() {}


void Game::run() {
    SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass()};

    while (isRunning) {
        handleEvents();

        if (auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

void Game::loadGameObjects() {
    std::vector<VulkanEngineModel::Vertex> vertices{
            {{0.0f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
    };

    auto engineModel = std::make_shared<VulkanEngineModel>(engineDevice, vertices);

    auto triangle = GameObject::createGameObject();
    triangle.model = engineModel;
    triangle.color = {0.f, 0.f, 1.f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {1.f, 1.f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                scrollAmount = scrollAmount + 10;
            } else if (event.wheel.y < 0) {
                scrollAmount = scrollAmount - 10;
            }
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
            }
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_Window *win = SDL_GetWindowFromID(event.window.windowID);
                int w;
                int h;
                SDL_GetWindowSize(win, &w, &h);
                window.onWindowResized(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
            }
        default:
            break;
    }

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[SDL_SCANCODE_W]) {
        cameraPos.y -= 20;
    }
    if (keystate[SDL_SCANCODE_A]) {
        cameraPos.x -= 20;
    }
    if (keystate[SDL_SCANCODE_S]) {
        cameraPos.y += 20;
    }
    if (keystate[SDL_SCANCODE_D]) {
        cameraPos.x += 20;
    }

    if (keystate[SDL_SCANCODE_E]) {
        cameraRot += 5;
        if (cameraRot > 360) cameraRot = cameraRot - 360;
    }
    if (keystate[SDL_SCANCODE_Q]) {
        cameraRot -= 5;
        if (cameraRot < 0) cameraRot = cameraRot + 360;
    }

    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

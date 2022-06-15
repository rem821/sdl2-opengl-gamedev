#include "Game.h"

Game::Game() {
    loadGameObjects();

    isRunning = true;
    run();
}

Game::~Game() {}


void Game::run() {
    SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass()};
    Camera camera{};

    //camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = GameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (isRunning) {
        handleEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

void Game::loadGameObjects() {
    std::shared_ptr<VulkanEngineModel> game_object = VulkanEngineModel::createModelFromFile(engineDevice, "models/smooth_vase.obj");

    auto object = GameObject::createGameObject();
    object.model = game_object;
    object.transform.translation = {.0f, .0f, 2.5f};
    object.transform.scale = {0.5f, 0.5f, 0.5f};

    gameObjects.push_back(std::move(object));
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
                //scrollAmount = scrollAmount + 10;
            } else if (event.wheel.y < 0) {
                //scrollAmount = scrollAmount - 10;
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

    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

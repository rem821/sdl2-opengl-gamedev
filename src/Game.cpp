#include "Game.h"

// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::unique_ptr<VulkanEngineModel> createCubeModel(VulkanEngineDevice &device, glm::vec3 offset) {
    VulkanEngineModel::Builder modelBuilder{};
    modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  .5f},   {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f},   {.9f, .9f, .9f}},
            {{-.5f, .5f,  -.5f},  {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f,  -.5f, -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .8f, .1f}},
            {{.5f,  -.5f, .5f},   {.8f, .8f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .8f, .1f}},

            // top face (purple)
            {{-.5f, -.5f, -.5f},  {.9f, .1f, .9f}},
            {{.5f,  -.5f, .5f},   {.9f, .1f, .9f}},
            {{-.5f, -.5f, .5f},   {.9f, .1f, .9f}},
            {{.5f,  -.5f, -.5f},  {.9f, .1f, .9f}},

            // bottom face (red)
            {{-.5f, .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .1f, .1f}},
            {{-.5f, .5f,  .5f},   {.8f, .1f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  .5f,  0.5f},  {.1f, .1f, .8f}},
            {{-.5f, .5f,  0.5f},  {.1f, .1f, .8f}},
            {{.5f,  -.5f, 0.5f},  {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  -.5f, -0.5f}, {.1f, .8f, .1f}},
    };
    for (auto &v: modelBuilder.vertices) {
        v.position += offset;
    }

    modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
                            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    return std::make_unique<VulkanEngineModel>(device, modelBuilder);
}

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
    std::shared_ptr<VulkanEngineModel> cubeModel = createCubeModel(engineDevice, {.0f, .0f, .0f});

    auto cube = GameObject::createGameObject();
    cube.model = cubeModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube));

    auto cube2 = GameObject::createGameObject();
    cube2.model = cubeModel;
    cube2.transform.translation = {-0.8f, .0f, 2.5f};
    cube2.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube2));

    auto cube3 = GameObject::createGameObject();
    cube3.model = cubeModel;
    cube3.transform.translation = {0.8f, .0f, 2.5f};
    cube3.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube3));

    auto cube4 = GameObject::createGameObject();
    cube4.model = cubeModel;
    cube4.transform.translation = {.0f, 0.8f, 2.5f};
    cube4.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube4));

    auto cube5 = GameObject::createGameObject();
    cube5.model = cubeModel;
    cube5.transform.translation = {.0f, -0.8f, 2.5f};
    cube5.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube5));

    auto cube6 = GameObject::createGameObject();
    cube6.model = cubeModel;
    cube6.transform.translation = {-0.8f, -0.8f, 2.5f};
    cube6.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube6));

    auto cube7 = GameObject::createGameObject();
    cube7.model = cubeModel;
    cube7.transform.translation = {0.8f, -0.8f, 2.5f};
    cube7.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube7));

    auto cube8 = GameObject::createGameObject();
    cube8.model = cubeModel;
    cube8.transform.translation = {0.8f, 0.8f, 2.5f};
    cube8.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube8));

    auto cube9 = GameObject::createGameObject();
    cube9.model = cubeModel;
    cube9.transform.translation = {-.8f, 0.8f, 2.5f};
    cube9.transform.scale = {.5f, .5f, .5f};

    gameObjects.push_back(std::move(cube9));
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

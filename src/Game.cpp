#include "Game.h"

Game::Game() {
    globalPool = VulkanEngineDescriptorPool::Builder(engineDevice)
            .setMaxSets(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

    loadGameObjects();

    isRunning = true;
    run();
}

Game::~Game() {}


void Game::run() {
    std::vector<std::unique_ptr<VulkanEngineBuffer>> uboBuffers(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<VulkanEngineBuffer>(
                engineDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        uboBuffers[i]->map();
    };

    auto globalSetLayout = VulkanEngineDescriptorSetLayout::Builder(engineDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        VulkanEngineDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    PointLightSystem pointLightSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

    Camera camera{};

    //camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
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
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects};
            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);
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
    object.transform.translation = {-.5f, .0f, 0.f};
    object.transform.scale = {5.f, 5.f, 5.f};

    gameObjects.emplace(object.getId(), std::move(object));

    std::shared_ptr<VulkanEngineModel> game_object2 = VulkanEngineModel::createModelFromFile(engineDevice, "models/flat_vase.obj");

    auto object2 = GameObject::createGameObject();
    object2.model = game_object2;
    object2.transform.translation = {.5f, .0f, 0.f};
    object2.transform.scale = {5.f, 5.f, 5.f};

    gameObjects.emplace(object2.getId(), std::move(object2));

    /*
    std::shared_ptr<VulkanEngineModel> suzane = VulkanEngineModel::createModelFromFile(engineDevice, "models/Suzane.obj");

    for (int i = 0; i < 50; i++) {
        for (int j = -50; j < 50; j++) {
            auto suz = GameObject::createGameObject();
            suz.model = suzane;
            suz.transform.translation = {2.75f * j, -2.2f * i, 2.5f};
            suz.transform.scale = {1.f, 1.f, 1.f};

            gameObjects.emplace(suz.getId(), std::move(suz));
        }
    }
     */


    std::shared_ptr<VulkanEngineModel> game_floor = VulkanEngineModel::createModelFromFile(engineDevice, "models/quad.obj");

    auto floor = GameObject::createGameObject();
    floor.model = game_floor;
    floor.transform.translation = {.0f, .1f, .0f};
    floor.transform.scale = {5.f, 1.f, 5.f};

    gameObjects.emplace(floor.getId(), std::move(floor));


    std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}
    };

    for (int i = 0; i < lightColors.size(); i++) {
        auto pointLight = GameObject::makePointLight(0.2f);
        pointLight.color = lightColors[i];
        auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                {0.f, -2.f, 1.f});
        pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        pointLight.transform.translation.y -= 1.f;
        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
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

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

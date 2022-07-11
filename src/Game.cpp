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

void Game::run() {
    // Init all the necessary systems
    std::vector<std::unique_ptr<VulkanEngineBuffer>> uboBuffers(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (auto &uboBuffer: uboBuffers) {
        uboBuffer = std::make_unique<VulkanEngineBuffer>(
                engineDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        uboBuffer->map();
    }

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

    SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), VK_POLYGON_MODE_FILL,
                                          VK_CULL_MODE_BACK_BIT};
    SimpleRenderSystem simpleLineRenderSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), VK_POLYGON_MODE_LINE,
                                              VK_CULL_MODE_NONE};
    PointLightSystem pointLightSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    Camera camera{};

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transform.translation = {(MAP_HEIGHT / 2) + (CHUNK_SIZE / 2), -50.0f, (MAP_WIDTH / 2) + (CHUNK_SIZE / 2)};
    viewerObject.transform.rotation = {-0.4f, 0.8f, .0f};
    KeyboardMovementController cameraController{};

    auto frameStartTime = std::chrono::high_resolution_clock::now();

    while (isRunning) {
        auto newFrameStartTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newFrameStartTime - frameStartTime).count();
        frameStartTime = newFrameStartTime;

        handleEvents();
        chunkManager.loadChunksAroundPlayerAsync({viewerObject.transform.translation.z, viewerObject.transform.translation.x}, 2);

        // Move newly loaded chunks into gameObjects
        for (auto &chunk: chunkManager.getVisibleChunks()) {
            if (gameObjects.emplace(chunk.second.getId(), std::move(chunk.second)).second) {
                glm::uvec2 pos = {chunk.second.transform.translation.z, chunk.second.transform.translation.x};
                GameObject border = chunkManager.getChunkBorders(pos);
                unsigned int borderId = border.getId();
                if (gameObjects.emplace(borderId, std::move(border)).second) {
                    chunkBorderIds.push_back(borderId);
                }
            }
        }

        // Move camera
        cameraController.moveInPlaneXZ(frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects};

            debugGui.showWindow(frameInfo, window.sdlWindow(), gameObjects, chunkBorderIds);

            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render -> Render solid objects first
            renderer.beginSwapChainRenderPass(commandBuffer);

            simpleRenderSystem.renderGameObjects(frameInfo);
            simpleLineRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);
            debugGui.render(commandBuffer);

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.getDevice());
}

void Game::loadGameObjects() {
    for (int i = 0; i < 5; i++) {
        auto pointLight = GameObject::makePointLight(5000.0f);
        pointLight.color = {0.609f, 0.18f, 0.207f};
        pointLight.transform.scale = {10.0f, 10.f, 10.f};
        pointLight.transform.translation.y -= 500.0f;
        pointLight.transform.translation.z += (float) i * (MAP_WIDTH / 3.0f);
        pointLight.transform.translation.x += MAP_HEIGHT / 2.0f;

        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }

    for (int i = 0; i < 5; i++) {
        auto pointLight = GameObject::makePointLight(5000.0f);
        pointLight.color = {0.609f, 0.18f, 0.207f};
        pointLight.transform.scale = {10.0f, 10.f, 10.f};
        pointLight.transform.translation.y -= 500.0f;
        pointLight.transform.translation.z += (float) i * (MAP_WIDTH / 3.0f);
        pointLight.transform.translation.x += MAP_HEIGHT;

        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        debugGui.processEvent(event);

        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
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
    }

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}
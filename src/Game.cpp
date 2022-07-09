#include "Game.h"


Game::Game() {
    globalPool = VulkanEngineDescriptorPool::Builder(engineDevice)
            .setMaxSets(VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

    loadGameObjects();
    loadTerrain();

    isRunning = true;
    run();
}

Game::~Game() {
    ImGui_ImplVulkan_Shutdown();
}


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

    SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), VK_POLYGON_MODE_FILL,
                                          VK_CULL_MODE_BACK_BIT};
    SimpleRenderSystem simpleLineRenderSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), VK_POLYGON_MODE_LINE,
                                              VK_CULL_MODE_NONE};

    PointLightSystem pointLightSystem{engineDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

    Camera camera{};

    setupImGui();

    camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transform.translation = {0, -200.0f, 0};
    viewerObject.transform.rotation = {-0.4f, 0.8f, .0f};
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (isRunning) {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        handleEvents();
        //imgui new frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.sdlWindow());

        cameraController.moveInPlaneXZ(frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects};

            ImGui::NewFrame();
            showWindow(frameInfo);
            ImGui::Render();

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

            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.getDevice());
}

void Game::loadGameObjects() {
    /*
    std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}
    };

    auto pointLight = GameObject::makePointLight(5000.0f);
    pointLight.color = lightColors[0];
    pointLight.transform.scale = {10.0f, 10.f, 10.f};
    pointLight.transform.translation.y -= 300.0f;
    pointLight.transform.translation.z += 200.0f;
    pointLight.transform.translation.x += 200.0f;

    gameObjects.emplace(pointLight.getId(), std::move(pointLight));

    auto pointLight2 = GameObject::makePointLight(5000.0f);
    pointLight2.color = lightColors[1];
    pointLight2.transform.scale = {10.0f, 10.f, 10.f};
    pointLight2.transform.translation.y -= 300.0f;
    pointLight2.transform.translation.z += 400.0f;
    pointLight2.transform.translation.x += 200.0f;

    gameObjects.emplace(pointLight2.getId(), std::move(pointLight2));

    auto pointLight3 = GameObject::makePointLight(5000.0f);
    pointLight3.color = lightColors[2];
    pointLight3.transform.scale = {10.0f, 10.f, 10.f};
    pointLight3.transform.translation.y -= 300.0f;
    pointLight3.transform.translation.z += 50.0f;
    pointLight3.transform.translation.x += 200.0f;

    gameObjects.emplace(pointLight3.getId(), std::move(pointLight3));

    auto pointLight4 = GameObject::makePointLight(5000.0f);
    pointLight4.color = lightColors[3];
    pointLight4.transform.scale = {10.0f, 10.f, 10.f};
    pointLight4.transform.translation.y -= 300.0f;
    pointLight4.transform.translation.z += 600.0f;
    pointLight4.transform.translation.x += 200.0f;

    gameObjects.emplace(pointLight4.getId(), std::move(pointLight4));

    auto pointLight5 = GameObject::makePointLight(5000.0f);
    pointLight5.color = lightColors[4];
    pointLight5.transform.scale = {10.0f, 10.f, 10.f};
    pointLight5.transform.translation.y -= 300.0f;
    pointLight5.transform.translation.z += 800.0f;
    pointLight5.transform.translation.x += 200.0f;

    gameObjects.emplace(pointLight5.getId(), std::move(pointLight5));
    */

    for (int i = 0; i < 5; i++) {
        auto pointLight = GameObject::makePointLight(5000.0f);
        pointLight.color = {0.609f, 0.18f, 0.207f};
        pointLight.transform.scale = {10.0f, 10.f, 10.f};
        pointLight.transform.translation.y -= 500.0f;
        pointLight.transform.translation.z += i * (MAP_WIDTH / 3);
        pointLight.transform.translation.x += MAP_HEIGHT / 2;

        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }

    for (int i = 0; i < 5; i++) {
        auto pointLight = GameObject::makePointLight(5000.0f);
        pointLight.color = {0.609f, 0.18f, 0.207f};
        pointLight.transform.scale = {10.0f, 10.f, 10.f};
        pointLight.transform.translation.y -= 500.0f;
        pointLight.transform.translation.z += i * (MAP_WIDTH / 3);
        pointLight.transform.translation.x += MAP_HEIGHT;

        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}

void Game::loadTerrain() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 30; j++) {
            glm::uvec2 pos = {j * CHUNK_SIZE, i * CHUNK_SIZE};
            GameObject obj = chunkManager.getChunkGameObject(pos);
            gameObjects.emplace(obj.getId(), std::move(obj));

            GameObject borders = chunkManager.getChunkBorders(pos);
            chunkBordersId.push_back(borders.getId());
            gameObjects.emplace(borders.getId(), std::move(borders));
        }
    }

}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);

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
    }

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

void Game::setupImGui() {
    imguiPool = VulkanEngineDescriptorPool::Builder(engineDevice)
            .setMaxSets(1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000)
            .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
            .build();


    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForVulkan(window.sdlWindow());

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = engineDevice.getInstance();
    init_info.PhysicalDevice = engineDevice.getPhysicalDevice();
    init_info.Device = engineDevice.getDevice();
    init_info.QueueFamily = engineDevice.findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue = engineDevice.graphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = imguiPool->getPool();
    init_info.Subpass = 0;
    init_info.MinImageCount = VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = VulkanEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&init_info, renderer.getSwapChainRenderPass());

    VkCommandBuffer commandBuffer = engineDevice.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    engineDevice.endSingleTimeCommands(commandBuffer);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void Game::showWindow(FrameInfo frameInfo) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);


    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Runtime info", nullptr, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::Text("FrameTime: %f ms", frameInfo.frameTime * 1000);
    ImGui::Text("FPS: %f", 1 / frameInfo.frameTime);
    ImGui::Text("GameObjects: %lu", frameInfo.gameObjects.size());
    ImGui::Text("Camera position: x:%f, y:%f, z:%f", frameInfo.camera.getPosition().x, frameInfo.camera.getPosition().y, frameInfo.camera.getPosition().z);
    ImGui::Text("Camera rotation: yaw:%f, pitch:%f, roll:%f", frameInfo.camera.getRotation().x, frameInfo.camera.getRotation().y,
                frameInfo.camera.getRotation().z);

    if (ImGui::Button("Render chunk borders")) {
        for (auto id: chunkBordersId) {
            gameObjects.at(id).isActive = !gameObjects.at(id).isActive;
        }
    }

    uint32_t vertices = 0;
    for (auto &kv: gameObjects) {
        auto &obj = kv.second;
        if (obj.model == nullptr) continue;
        if (!obj.isActive) continue;

        vertices += obj.model->getVertexCount();
    }

    ImGui::Text("Vertices: %u", vertices);

    ImGui::End();
}

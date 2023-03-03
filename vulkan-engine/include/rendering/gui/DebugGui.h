//
// Created by Stanislav Svědiroh on 11.07.2022.
//
#pragma once

#include "../VulkanEngineFrameInfo.h"
#include "../VulkanEngineDescriptors.h"
#include "../VulkanEngineSwapChain.h"
#include "../VulkanEngineRenderer.h"
#include "../../CoordinateSystem.h"
#include "../../GlobalConfiguration.h"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl2.h>
#include <vector>

class DebugGui {
public:
    DebugGui(VulkanEngineDevice &engineDevice, VulkanEngineRenderer &renderer, SDL_Window *window);
    ~DebugGui();

    DebugGui(const DebugGui &) = delete;
    DebugGui &operator=(const DebugGui &) = delete;

    void showWindow(const FrameInfo &frameInfo, SDL_Window *window, GameObject::Map &gameObjects, const std::vector<uint32_t> &chunkBorderIds);

    void render(VkCommandBuffer &commandBuffer);
    void processEvent(SDL_Event event);
private:

    double movingFPSAverage = 0;
    bool chunkBordersShown = false;
    bool renderWireframes = false;

    std::unique_ptr<VulkanEngineDescriptorPool> imguiPool{};
};
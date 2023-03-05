//
// Created by standa on 3.3.23.
//
#pragma once

extern VulkanEngine::Application* VulkanEngine::CreateApplication();

int main() {
    VulkanEngine::Log::Init();
    CORE_INFO("Initialized Core logger!");
    CORE_INFO("Initialized Client logger!");

    auto app = VulkanEngine::CreateApplication();
    app->Run();
    delete app;
}
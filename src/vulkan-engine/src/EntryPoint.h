//
// Created by standa on 3.3.23.
//
#pragma once

extern VulkanEngine::Application* VulkanEngine::CreateApplication();

int main(int argc, char** argv) {
    auto app = VulkanEngine::CreateApplication();
    fmt::print("Welcome to the VulkanEngine!\n");

    app->Run();
    delete app;
}

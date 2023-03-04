//
// Created by standa on 3.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <rendering/Window.h>

namespace VulkanEngine {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    private:
        std::unique_ptr<Window> window_;
        bool isRunning_ = true;
    };

    // To be defined in client application
    Application* CreateApplication();
}
//
// Created by standa on 3.3.23.
//
#pragma once

#include "Core.h"

namespace VulkanEngine {

    class ENGINE_API Application {
    public:
        Application() {};
        virtual ~Application() {};

        void Run() {
            while(true);
        };
    };

    // To be defined in client application
    Application* CreateApplication();
}
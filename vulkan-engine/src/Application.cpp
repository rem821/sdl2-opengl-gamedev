//
// Created by standa on 4.3.23.
//
#include "Application.h"
#include "events/ApplicationEvent.h"

namespace VulkanEngine {

    Application::Application() = default;

    Application::~Application() = default;

    void Application::Run() {
        WindowResizeEvent e(1280, 720);
        CORE_TRACE(e);

        while(true);
    }

}

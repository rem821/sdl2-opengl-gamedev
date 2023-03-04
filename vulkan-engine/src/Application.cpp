//
// Created by standa on 4.3.23.
//
#include "Application.h"
#include "events/ApplicationEvent.h"

namespace VulkanEngine {

    Application::Application() {
        window_ = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application() = default;

    void Application::Run() {
        while(isRunning_) {
            window_->OnUpdate();
        }
    }

}

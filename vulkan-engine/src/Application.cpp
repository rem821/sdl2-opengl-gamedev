//
// Created by standa on 4.3.23.
//
#include "Application.h"
#include "events/ApplicationEvent.h"

namespace VulkanEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() {
        window_ = std::unique_ptr<Window>(Window::Create());
        window_->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application() = default;

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        CORE_INFO(e);
    }

    void Application::Run() {
        while(isRunning_) {
            window_->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        isRunning_ = false;
        return true;
    }

}

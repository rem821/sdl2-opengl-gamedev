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

    void Application::PushLayer(Layer *layer) {
        layerStack_.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *layer) {
        layerStack_.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        CORE_INFO(e);

        for(auto it = layerStack_.end(); it != layerStack_.begin(); ) {
            (*--it)->OnEvent(e);
            if(e.IsHandled())
                break;
        }
    }

    void Application::Run() {
        while(isRunning_) {

            for(Layer* layer : layerStack_)
                layer->OnUpdate();

            window_->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        isRunning_ = false;
        return true;
    }
}

//
// Created by standa on 4.3.23.
//
#include "Application.h"

#include <memory>
#include "events/ApplicationEvent.h"

namespace VulkanEngine {

    Application* Application::appInstance_ = nullptr;

    Application::Application() {
        CORE_ASSERT(!appInstance_, "Application already exists!")
        appInstance_ = this;

        window_ = std::unique_ptr<Window>(Window::Create());
        window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        vulkanDevice_ = std::make_unique<VulkanDevice>((GLFWwindow *) window_->GetNativeWindow(), true);
        vulkanRenderer_ = std::make_unique<VulkanRenderer>(*window_, *vulkanDevice_);
        globalPool_ = VulkanDescriptorPool::Builder(*vulkanDevice_)
                .SetMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
                .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
                .Build();

        imGuiLayer_ = new ImGuiLayer();
        PushOverlay(imGuiLayer_);
    }

    Application::~Application() = default;

    void Application::PushLayer(Layer *layer) {
        layerStack_.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *layer) {
        layerStack_.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

        // CORE_INFO(e);

        for (auto it = layerStack_.end(); it != layerStack_.begin();) {
            (*--it)->OnEvent(e);
            if (e.IsHandled())
                break;
        }
    }

    void Application::Run() {
        while (isRunning_) {
            if (auto commandBuffer = vulkanRenderer_->BeginFrame()) {
                vulkanRenderer_->BeginSwapChainRenderPass(commandBuffer);

                for (Layer *layer: layerStack_)
                    layer->OnUpdate(commandBuffer);

                imGuiLayer_->Begin();
                for (Layer *layer: layerStack_)
                    layer->OnImGuiRender();
                imGuiLayer_->End(commandBuffer);

                window_->OnUpdate();

                vulkanRenderer_->EndSwapChainRenderPass(commandBuffer);
                vulkanRenderer_->EndFrame();
            }
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
        isRunning_ = false;
        return true;
    }
}

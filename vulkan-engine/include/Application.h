//
// Created by standa on 3.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <rendering/Window.h>

#include <imgui/ImGuiLayer.h>

namespace VulkanEngine {

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        [[nodiscard]] inline Window& GetWindow() const { return *window_; }

        inline static Application& Get() { return *appInstance_; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> window_;
        ImGuiLayer* imGuiLayer_;

        bool isRunning_ = true;
        LayerStack layerStack_;

        static Application* appInstance_;
    };

    // To be defined in client application
    Application* CreateApplication();
}
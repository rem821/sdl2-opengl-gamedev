//
// Created by standa on 3.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <rendering/Window.h>
#include <rendering/VulkanDevice.h>
#include <rendering/VulkanRenderer.h>
#include <rendering/VulkanDescriptors.h>
#include <imgui/ImGuiLayer.h>

namespace VulkanEngine {

    class ENGINE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        [[nodiscard]] inline Window& GetWindow() const { return *window_; }
        [[nodiscard]] inline const VulkanDevice& GetDevice() const { return *vulkanDevice_; }
        [[nodiscard]] inline const VulkanRenderer& GetRenderer() const { return *vulkanRenderer_; }
        [[nodiscard]] inline VulkanDescriptorPool& GetDescriptorPool() const { return *globalPool_; }

        inline static Application& Get() { return *appInstance_; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> window_;
        ImGuiLayer* imGuiLayer_;

        std::unique_ptr<VulkanDevice> vulkanDevice_;
        std::unique_ptr<VulkanRenderer> vulkanRenderer_;
        std::unique_ptr<VulkanDescriptorPool> globalPool_;

        bool isRunning_ = true;
        LayerStack layerStack_;

        static Application* appInstance_;
    };

    // To be defined in client application
    Application* CreateApplication();
}
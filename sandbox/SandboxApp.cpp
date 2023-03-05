//
// Created by standa on 3.3.23.
//
#include "VulkanEngine.h"

class TestLayer : public VulkanEngine::Layer {
public:
    TestLayer() : Layer("Test") {}

    void OnUpdate(VkCommandBuffer commandBuffer) override {
        //CLIENT_INFO("TestLayer::Update");
    }

    void OnEvent(VulkanEngine::Event &event) override {
        //CLIENT_TRACE("TestLayer::OnEvent: {}", event);
    }
};


class SandboxApp : public VulkanEngine::Application {
public:
    SandboxApp() {
        PushLayer(new TestLayer());
        PushOverlay(new VulkanEngine::ImGuiLayer());
    }

    ~SandboxApp() = default;
};

VulkanEngine::Application *VulkanEngine::CreateApplication() {
    return new SandboxApp();
}
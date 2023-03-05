//
// Created by standa on 3.3.23.
//
#include "VulkanEngine.h"
#include "libs/imgui/imgui.h"

class TestLayer : public VulkanEngine::Layer {
public:
    TestLayer() : Layer("Test") {}

    void OnUpdate(VkCommandBuffer commandBuffer) override {
        if(VulkanEngine::Input::IsKeyPressed(VE_KEY_SPACE))
            CLIENT_TRACE("Space key is pressed!");
    }

    void OnEvent(VulkanEngine::Event &event) override {
        //CLIENT_TRACE("TestLayer::OnEvent: {}", event);
    }

    void OnImGuiRender() override {
        ImGui::Begin("Test");
        ImGui::Text("Hello world");
        ImGui::End();
    }
};


class SandboxApp : public VulkanEngine::Application {
public:
    SandboxApp() {
        PushLayer(new TestLayer());
    }

    ~SandboxApp() override = default;
};

VulkanEngine::Application *VulkanEngine::CreateApplication() {
    return new SandboxApp();
}
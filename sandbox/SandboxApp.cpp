//
// Created by standa on 3.3.23.
//
#include "VulkanEngine.h"


class SandboxApp : public VulkanEngine::Application {
public:
    SandboxApp() = default;

    ~SandboxApp() = default;
};

VulkanEngine::Application *VulkanEngine::CreateApplication() {
    return new SandboxApp();
}
//
// Created by standa on 5.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <Input.h>

namespace VulkanEngine {

    class LinuxInput : public Input {

    protected:
        bool IsKeyPressedImpl(int keycode) override;
        bool IsMouseButtonPressedImpl(int button) override;
        std::pair<float, float> GetMousePositionImpl() override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;

    };
}

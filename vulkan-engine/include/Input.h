//
// Created by standa on 5.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>

namespace VulkanEngine {

    class Input {
    public:
        inline static bool IsKeyPressed(int keycode) { return instance_->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return instance_->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return instance_->GetMousePositionImpl(); }
        inline static bool GetMouseX() { return instance_->GetMouseXImpl(); }
        inline static bool GetMouseY() { return instance_->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* instance_;
    };
}
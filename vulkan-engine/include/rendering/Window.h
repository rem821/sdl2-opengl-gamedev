//
// Created by Stanislav Svědiroh on 13.06.2022.
//
#pragma once

#include "precompiled_headers/PCH.h"

namespace VulkanEngine {
    struct WindowProps {
        std::string Title;
        unsigned int Width, Height;

        explicit WindowProps(std::string title = "Vulkan Engine", unsigned int width = 1920, unsigned int height = 1080)
                : Title(std::move(title)), Width(width), Height(height) {}
    };

    class ENGINE_API Window {
    public:
        using EventCallbackFn = std::function<void(Event &)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual unsigned int GetWidth() const = 0;

        [[nodiscard]] virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

        virtual void SetVSync(bool enabled) = 0;

        [[nodiscard]] virtual bool IsVSync() const = 0;

        static Window *Create(const WindowProps &props = WindowProps());
    };
}

//
// Created by standa on 4.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <GLFW/glfw3.h>
#include <rendering/Window.h>
#include <renderer/GraphicsContext.h>

namespace VulkanEngine {

    class LinuxWindow : public Window {
    public:
        explicit LinuxWindow(const WindowProps &props);

        ~LinuxWindow() override;

        void OnUpdate() override;

        [[nodiscard]] inline unsigned int GetWidth() const override { return data_.Width; }

        [[nodiscard]] inline unsigned int GetHeight() const override { return data_.Height; }

        inline void SetEventCallback(const EventCallbackFn &callback) override { data_.EventCallback = callback; }

        [[nodiscard]] void *GetNativeWindow() const override { return window_; }

        [[nodiscard]] GraphicsContext *GetGraphicsContext() const override { return graphicsContext_; }

    private:
        virtual void Init(const WindowProps &props);

        virtual void Shutdown();

        GLFWwindow *window_{};
        GraphicsContext *graphicsContext_{};

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;

            EventCallbackFn EventCallback;
        };

        WindowData data_;
    };
}
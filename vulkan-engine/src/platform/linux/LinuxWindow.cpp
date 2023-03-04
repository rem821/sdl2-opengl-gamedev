//
// Created by standa on 4.3.23.
//
#include "platform/linux/LinuxWindow.h"

namespace VulkanEngine {

    static bool GLFWInitialized_ = false;

    Window* Window::Create(const WindowProps &props) {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps &props) {
        Init(props);
    }

    LinuxWindow::~LinuxWindow() {
        glfwDestroyWindow(window_);
    }

    void LinuxWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(window_);
    }

    void LinuxWindow::SetVSync(bool enabled) {
        glfwSwapInterval((int) enabled);
        data_.VSync = enabled;
    }

    void LinuxWindow::Init(const WindowProps &props) {
        data_.Title = props.Title;
        data_.Width = props.Width;
        data_.Height = props.Height;

        CORE_INFO("Creating window {} ({} {})", props.Title, props.Width, props.Height);

        if(!GLFWInitialized_) {
            int success = glfwInit();
            CORE_ASSERT(success, "Could not initialize GLFW!")

            GLFWInitialized_ = true;
        }

        window_ = glfwCreateWindow((int)props.Width, (int)props.Height, data_.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, &data_);
        SetVSync(true);
    }

    void LinuxWindow::Shutdown() {

    }
}
//
// Created by standa on 4.3.23.
//
#include "platform/linux/LinuxWindow.h"

namespace VulkanEngine {

    static bool GLFWInitialized_ = false;

    static void GLFWErrorCallback(int error, const char *description) {
        CORE_ERROR("GLFW error ({}): {}", error, description);
    }

    Window *Window::Create(const WindowProps &props) {
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
    }

    void LinuxWindow::Init(const WindowProps &props) {
        data_.Title = props.Title;
        data_.Width = props.Width;
        data_.Height = props.Height;

        CORE_INFO("Creating window {} ({} {})", props.Title, props.Width, props.Height);

        if (!GLFWInitialized_) {
            int success = glfwInit();
            CORE_ASSERT(success, "Could not initialize GLFW!")
            glfwSetErrorCallback(GLFWErrorCallback);

            CORE_ASSERT(glfwVulkanSupported(), "GLFW Doesn't support Vulkan!")
            GLFWInitialized_ = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window_ = glfwCreateWindow((int) props.Width, (int) props.Height, data_.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window_, &data_);

        // Set GLFW Callbacks
        glfwSetWindowSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            data.Width = width;
            data.Height = height;
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            WindowData &data = *(WindowData *)  (window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window_, [](GLFWwindow *window, unsigned int keyCode) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
            KeyTypedEvent event((int) keyCode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int button, int action, int mods) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window_, [](GLFWwindow *window, double xOffset, double yOffset) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xPosition, double yPosition) {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPosition, (float) yPosition);
            data.EventCallback(event);
        });
    }

    void LinuxWindow::Shutdown() {

    }
}
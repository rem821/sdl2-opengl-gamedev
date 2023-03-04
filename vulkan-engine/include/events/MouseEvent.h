//
// Created by standa on 4.3.23.
//
#pragma once

#include "Event.h"

namespace VulkanEngine {

    class ENGINE_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y): mouseX_(x), mouseY_(y) {}

        [[nodiscard]] inline float GetX() const { return mouseX_; }
        [[nodiscard]] inline float GetY() const { return mouseY_; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mouseX_ << ", " << mouseY_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float mouseX_, mouseY_;
    };

    class ENGINE_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset): xOffset_(xOffset), yOffset_(yOffset) {}

        [[nodiscard]] inline float GetXOffset() const { return xOffset_; }
        [[nodiscard]] inline float GetYOffset() const { return yOffset_; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << xOffset_ << ", " << yOffset_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float xOffset_, yOffset_;
    };

    class ENGINE_API MouseButtonEvent : public Event {
    public:
        [[nodiscard]] inline int GetMouseButton() const { return button_; }

        EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)
    protected:
        explicit MouseButtonEvent(int button): button_(button) {}

        int button_;
    };

    class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(int button): MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}
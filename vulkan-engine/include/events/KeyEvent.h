//
// Created by standa on 4.3.23.
//
#pragma once

#include "Event.h"

 namespace VulkanEngine {

    class ENGINE_API KeyEvent : public Event {
    public:
        [[nodiscard]] inline int GetKeyCode() const { return keyCode_; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        explicit KeyEvent(int keyCode): keyCode_(keyCode) {}

        int keyCode_;
    };

    class ENGINE_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keyCode, int repeatCount): KeyEvent(keyCode), repeatCount_(repeatCount) {}

        [[nodiscard]] inline int GetRepeatCount() const { return repeatCount_; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode_ << " (" << repeatCount_ << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int repeatCount_;
    };

    class ENGINE_API KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keyCode): KeyEvent(keyCode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}
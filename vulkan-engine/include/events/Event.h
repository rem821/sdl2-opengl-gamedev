//
// Created by standa on 4.3.23.
//
#pragma once

#include "precompiled_headers/PCH.h"

namespace VulkanEngine {


    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication =  BIT(0),
        EventCategoryInput =        BIT(1),
        EventCategoryKeyboard =     BIT(2),
        EventCategoryMouse =        BIT(3),
        EventCategoryMouseButton =  BIT(4),
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }\

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class ENGINE_API Event {
        friend class EventDispatcher;
    public:
        [[nodiscard]] virtual EventType GetEventType() const = 0;
        [[nodiscard]] virtual const char* GetName() const = 0;
        [[nodiscard]] virtual int GetCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string ToString() const { return GetName(); }

        [[nodiscard]] inline bool IsInCategory(EventCategory category) const {
            return GetCategoryFlags() & category;
        }

        [[nodiscard]] inline bool IsHandled() const { return handled_; }

    protected:
        bool handled_ = false;
    };

    class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        explicit EventDispatcher(Event& event): event_(event) {

        }

        template<typename T>
        bool Dispatch(EventFn<T> func) {
            if(event_.GetEventType() == T::GetStaticType()) {
                event_.handled_ = func(*(T*)&event_);
                return true;
            }
            return false;
        }
    private:
        Event& event_;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }
}
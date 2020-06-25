//
// Created by Koko on 25.05.2020.
//

#ifndef UNTITLED1_EVENT_H
#define UNTITLED1_EVENT_H

#define BIT(x) (1 << x)

#include "MouseButtons.h"
#include "KeyboardButtons.h"


enum class EventType {
    MouseMoved = 0,
    MouseButtonPressed,
    MouseButtonReleased,
    KeyPressed,
    KeyReleased
};

enum EventCategory {
    EventCategoryMouse = 0,
    EventCategoryKeyboard = 1
};


#define EVENT_CLASS_CATEGORY(category) virtual int getCategories() const override { return category; }


class Event {
    bool handled = false;

public:
    virtual EventType getEventType() const = 0;
    virtual int getCategories() const = 0;
};


class KeyPressedEvent : public Event {
    KeyboardButton button;

public:
    explicit KeyPressedEvent(KeyboardButton _button);

    EventType getEventType() const override {
        return EventType::KeyPressed;
    }

    KeyboardButton getButton() const {
        return button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard);
};


class KeyReleasedEvent : public Event {
    KeyboardButton button;

public:
    explicit KeyReleasedEvent(KeyboardButton _button);

    EventType getEventType() const override {
        return EventType::KeyReleased;
    }

    KeyboardButton getButton() const {
        return button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard);
};


class MouseButtonPressedEvent : public Event {
    MouseButton button;

public:
    explicit MouseButtonPressedEvent(MouseButton _button);

    EventType getEventType() const override {
        return EventType::MouseButtonPressed;
    }

    MouseButton getButton() const {
        return button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse);
};


class MouseButtonReleasedEvent : public Event {
    MouseButton button;

public:
    explicit MouseButtonReleasedEvent(MouseButton _button);

    EventType getEventType() const override {
        return EventType::MouseButtonReleased;
    }

    MouseButton getButton() const {
        return button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse);
};


class MouseMovedEvent : public Event {
    double m_mouseX;
    double m_mouseY;

public:
    MouseMovedEvent(double x, double y);

    double getX() const;
    double getY() const;

    EventType getEventType() const override {
        return EventType::MouseMoved;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse)
};


#endif //UNTITLED1_EVENT_H

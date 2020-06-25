//
// Created by Koko on 25.05.2020.
//

#include "Event.h"

MouseMovedEvent::MouseMovedEvent(double x, double y) :m_mouseX(x), m_mouseY(y) { }

double MouseMovedEvent::getX() const {
    return m_mouseX;
}

double MouseMovedEvent::getY() const {
    return m_mouseY;
}


MouseButtonPressedEvent::MouseButtonPressedEvent(MouseButton _button) : button(_button) {}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(MouseButton _button) : button(_button) {}

KeyPressedEvent::KeyPressedEvent(KeyboardButton _button) : button(_button) {}

KeyReleasedEvent::KeyReleasedEvent(KeyboardButton _button) : button(_button) {}
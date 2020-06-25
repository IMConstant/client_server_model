//
// Created by Koko on 25.05.2020.
//

#include "WindowEventHandler.h"

static void cursorPositionCallback(GLFWwindow *window, double x, double y) {
    auto *windowEventHandler = reinterpret_cast<WindowEventHandler *>(glfwGetWindowUserPointer(window));

    MouseMovedEvent event(x, y);
    windowEventHandler->onEvent(event);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    auto *windowEventHandler = reinterpret_cast<WindowEventHandler *>(glfwGetWindowUserPointer(window));

    MouseButton btn = (button == GLFW_MOUSE_BUTTON_LEFT ? MOUSE_BUTTON_LEFT : MOUSE_BUTTON_RIGHT);

    if (action == GLFW_PRESS) {
        MouseButtonPressedEvent event(btn);
        windowEventHandler->onEvent(event);
    }
    else if (action == GLFW_RELEASE) {
        MouseButtonReleasedEvent event(btn);
        windowEventHandler->onEvent(event);
    }
}

static void keyboardButtonCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto *windowEventHandler = reinterpret_cast<WindowEventHandler *>(glfwGetWindowUserPointer(window));

    auto btn = static_cast<KeyboardButton>(key);

    if (action == GLFW_PRESS) {
        KeyPressedEvent event(btn);
        windowEventHandler->onEvent(event);
    }
    else if (action == GLFW_RELEASE) {
        KeyReleasedEvent event(btn);
        windowEventHandler->onEvent(event);
    }
}


WindowEventHandler::WindowEventHandler(GLFWwindow *window) : m_window(window) {
    glfwSetWindowUserPointer(m_window, this);

    initializeEventCallBacks();
}

bool WindowEventHandler::initializeEventCallBacks() {
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

    glfwSetKeyCallback(m_window, keyboardButtonCallback);

    return true;
}

void WindowEventHandler::onEvent(Event &event) {
    Application::getInstance()->onEvent(event);
}
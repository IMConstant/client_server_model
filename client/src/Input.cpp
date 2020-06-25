//
// Created by Koko on 01.06.2020.
//

#include "Input.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <memory>

std::unique_ptr<Input> Input::s_instance = std::unique_ptr<Input>(new Input());


bool Input::isMouseButtonPressed(MouseButton button) {
    GLFWwindow *window = Application::getInstance()->getWindow();
    auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

    return state == GLFW_PRESS;
}

bool Input::isKeyPressed(KeyboardButton button) {
    GLFWwindow *window = Application::getInstance()->getWindow();
    auto state = glfwGetKey(window, static_cast<int32_t>(button));

    return state == GLFW_PRESS;
}

std::pair<float, float> Input::getMousePosition() {
    GLFWwindow *window = Application::getInstance()->getWindow();

    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}
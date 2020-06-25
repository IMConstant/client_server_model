//
// Created by Koko on 01.06.2020.
//

#ifndef UNTITLED1_INPUT_H
#define UNTITLED1_INPUT_H

#include <memory>

#include "MouseButtons.h"
#include "KeyboardButtons.h"


class Input {
public:
    Input() = default;
    Input(const Input &other) = delete;
    Input &operator=(const Input &other) = delete;

    static bool isMouseButtonPressed(MouseButton button);
    static bool isKeyPressed(KeyboardButton button);
    static std::pair<float, float> getMousePosition();

private:
    static std::unique_ptr<Input> s_instance;
};


#endif //UNTITLED1_INPUT_H

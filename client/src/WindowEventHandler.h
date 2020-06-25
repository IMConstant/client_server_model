//
// Created by Koko on 25.05.2020.
//

#ifndef UNTITLED1_WINDOWEVENTHANDLER_H
#define UNTITLED1_WINDOWEVENTHANDLER_H

#include "Application.h"
#include "Event.h"

class Application;


class WindowEventHandler {
    GLFWwindow *m_window;

public:
    explicit WindowEventHandler(GLFWwindow *window = nullptr);

    bool initializeEventCallBacks();
    void onEvent(Event &event);
};


#endif //UNTITLED1_WINDOWEVENTHANDLER_H

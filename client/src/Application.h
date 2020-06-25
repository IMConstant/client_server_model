//
// Created by Koko on 25.05.2020.
//

#ifndef UNTITLED1_APPLICATION_H
#define UNTITLED1_APPLICATION_H

#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "WindowEventHandler.h"
#include <GLFW/glfw3.h>

#include <socket/Socket.h>
#include <mutex>
#include <thread>
#include <atomic>
#include <list>

#include "Input.h"
#include "ParticleSystem.h"

#include "../../core/managers/EntityManager.h"

#include "../../core/objects/components/components.h"
#include "../../core/objects/Entity.h"

#include "../../scripts/UserControlComponent.h"


int main(int argc, char **argv);

class WindowEventHandler;
class Event;


class Application {
    friend int ::main(int argc, char **argv);
    static Application *s_instance;

    GLFWwindow *m_window;
    WindowEventHandler *m_eventHandler;

    std::atomic_bool shouldClose;
    std::string send_buffer;
    std::string recv_buffer;

    std::mutex m_guard;
    std::mutex m_connection_guard;
    std::thread m_chat_thread;
    std::atomic_bool m_connection;

    double m_mouseX;
    double m_mouseY;

    ParticleSystem particleSystem;

public:
    Application();
    ~Application();

    void onEvent(Event &event);
    static Application *getInstance();
    GLFWwindow *getWindow() const {
        return m_window;
    }

    ParticleSystem &getParticleSystem() {
        return particleSystem;
    }

private:
    void chat_thread_handler();

    void initializeImGui();
    void initializeOpenGL();
    void run();
};


#endif //UNTITLED1_APPLICATION_H

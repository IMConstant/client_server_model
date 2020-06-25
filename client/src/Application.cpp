//
// Created by Koko on 25.05.2020.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"
#include <thread>
#include <functional>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


Application *Application::s_instance = nullptr;


Application::Application() {
    if (s_instance) {
        throw;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    s_instance = this;
    m_window = glfwCreateWindow(800, 600, "Application", nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    glewInit();
    glfwSwapInterval(1);

    m_eventHandler = new WindowEventHandler(m_window);

    initializeImGui();

    shouldClose = false;
    m_connection = false;
}

Application::~Application() {
    shouldClose = true;
}

void Application::initializeOpenGL() {

}

void Application::initializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    ImFontConfig font_config;
    font_config.OversampleH = 1; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;

    static const ImWchar ranges[] =
            {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x044F, // Cyrillic
                    0,
            };

    io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\Tahoma.ttf)", 18.0f, &font_config, ranges);

    //ImFont* font = io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\ROCK.ttf)", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
}

Application *Application::getInstance() {
    return s_instance;
}

void Application::onEvent(Event &event) {
    if (event.getEventType() == EventType::MouseMoved) {
        auto &mouseEvent = dynamic_cast<MouseMovedEvent &>(event);

        m_mouseX = mouseEvent.getX();
        m_mouseY = mouseEvent.getY();
    }
    else if (event.getEventType() == EventType::MouseButtonPressed) {

    }
    else if (event.getEventType() == EventType::MouseButtonReleased) {

    }
}


void Application::chat_thread_handler() {
    bool connected = false;
    bool send_ready = false;
    std::string snd_buf;

    TCPSocket socket{};

    while (!shouldClose) {
        if (!connected) {
            connected = socket.connect("25.24.79.180", 7777);
            std::cout << WSAGetLastError() << std::endl;

            if (connected) {
                m_connection = connected;
            }
            else {
                continue;
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_guard);

            if (send_buffer.length() > 0) {
                snd_buf = send_buffer;
                send_ready = true;
            }
        }

        if (send_ready && socket.send(snd_buf) > 0) {
            {
                std::lock_guard<std::mutex> lock(m_guard);
                send_buffer.clear();
                send_ready = false;
            }
        }

        {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(socket.native(), &set);

            if (WSAGetLastError() != 0) {
                connected = false;
                m_connection = false;

                shutdown(socket.native(), SD_BOTH);
                closesocket(socket.native());

                socket = TCPSocket();
                continue;
            }

            auto timeout = TIMEVAL();
            timeout.tv_sec = 0;
            timeout.tv_usec = 1000;

            select(socket.native() + 1, &set, nullptr, nullptr, &timeout);

            if (FD_ISSET(socket.native(), &set)) {
                char receive_buffer[128] = {0};
                socket.recv(receive_buffer, 128);
//                std::cout << "received" << std::endl;
//                std::cout << receive_buffer << std::endl;

                {
                    std::lock_guard<std::mutex> lock(m_guard);
                    recv_buffer += std::string(receive_buffer) + "\n";
                }
            }
        }
    }

    shutdown(socket.native(), SD_BOTH);
    closesocket(socket.native());
}


void Application::run() {
    WSAData wsa{};

    if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0) {
        std::cout << "WSA startup success" << std::endl;
    }

    setlocale(LC_ALL, "Russian");
    std::wcout << L"Привет";

    bool showWindow = true;
    bool menuHovered = false;

    std::list<std::string> messages;
    std::string chat_buffer("\n");
    std::string name = "User";

    int emit_count = 1;

    ImVec4 back_color(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 particle_begin_color(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 particle_end_color(1.0f, 1.0f, 0.0f, 1.0f);

    m_chat_thread = std::thread(std::bind(&Application::chat_thread_handler, this));

    UDPSocket udp_socket{};
    udp_socket.open("", 7779);
    udp_socket.setBlocking(false);

    auto *spaceShip = new core::Entity();
    spaceShip->addComponent<core::PositionComponent>(core::vec2(0, 0));
    spaceShip->addComponent<core::Transformable>();
    spaceShip->addComponent<core::ShapeComponent>();
    spaceShip->addComponent<UserControlComponent>(glm::vec2(0, 1));

    core::EntityManager::getInstance()->add(spaceShip);

    core::Entity spaceShip2{};
    spaceShip2.addComponent<core::PositionComponent>(core::vec2(400, 400));
    spaceShip2.addComponent<core::Transformable>();
    spaceShip->addComponent<core::ShapeComponent>();
    spaceShip2.addComponent<UserControlComponent>(glm::vec2(0, 1));

    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Once);

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove);
        menuHovered = ImGui::IsWindowHovered();

        {
            char name_string[16] { 0 };
            ImVec4 connection_color;
            std::string connection_string;

            float fps = ImGui::GetIO().Framerate;
            float p = fps / 60.0f;
            ImVec4 fps_color(1.0f - p, p, 0.0f, 1.0f);

            if (m_connection == true) {
                connection_color = { 0.0f, 1.0f, 0.0f, 1.0f };
                connection_string = "online";
            }
            else {
                connection_color = { 1.0f, 0.0f, 0.0f, 1.0f };\
                connection_string = "offline";
            }

            if (ImGui::CollapsingHeader("Settigns")) {
                ImGui::Text(u8"Name: %s", name.c_str());
                ImGui::Text("status:");
                ImGui::SameLine();
                ImGui::TextColored(connection_color, "%s", connection_string.c_str());
                if (ImGui::InputText("New name", name_string, sizeof(name_string), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    {
                        std::lock_guard<std::mutex> lock(m_guard);
                        send_buffer += name + " changed name to " + name_string;
                    }

                    name = name_string;
                }
                ImGui::Text("Background color:");
                ImGui::ColorEdit3("", reinterpret_cast<float *>(&back_color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::Text("Particle colors:");
                ImGui::ColorEdit3("begin color", reinterpret_cast<float *>(&particle_begin_color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SameLine();
                ImGui::ColorEdit3("end color", reinterpret_cast<float *>(&particle_end_color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SliderInt("emit count", &emit_count, 1, 100);
                ImGui::Text("particles %i", particleSystem.getParticlesCount());
                ImGui::Text("fps:");
                ImGui::SameLine();
                ImGui::TextColored(fps_color, "%.1f", fps);
            }
        }

        {
            {
                std::lock_guard<std::mutex> lock(m_guard);

                if (recv_buffer.length() > 0) {
                    messages.push_front(recv_buffer);
                    recv_buffer.clear();
                }
            }

            char message[128] = { 0 };

            if (ImGui::CollapsingHeader("Chat")) {
                if (ImGui::InputText("", message, sizeof(message), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (strlen(message) > 0) {
                        std::string data = name + ": " + message;

                        {
                            std::lock_guard<std::mutex> lock(m_guard);
                            send_buffer += data;
                        }
                    }

                    ImGui::SetKeyboardFocusHere(2);
                }

                menuHovered |= ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

                for (auto &msg: messages) {
                    ImGui::TextWrapped("%s", msg.c_str());
                }
            }
        }

        ImGui::End();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(back_color.x, back_color.y, back_color.z, back_color.w);

        glm::mat4 projection = glm::ortho(-(float)display_w / 2.0f, (float)display_w / 2.0f, -(float)display_h / 2.0f, (float)display_h / 2.0f, -1.0f, 1.0f);
        auto &ship_position = spaceShip->getComponent<core::PositionComponent>().position;
        projection = glm::ortho(ship_position.x - (float)display_w / 2.0f, ship_position.x + (float)display_w / 2.0f,
                ship_position.y - (float)display_h / 2.0f, ship_position.y + (float)display_h / 2.0f, -1.0f, 1.0f);
        glm::mat4 modelView(1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(reinterpret_cast<float *>(&projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(reinterpret_cast<float *>(&modelView));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT) && !menuHovered) {
            if (m_connection) {
                glm::vec2 coords = { Input::getMousePosition().first, Input::getMousePosition().second };
                char coords_data[20] = { 0 };
                *reinterpret_cast<unsigned char *>(coords_data) = 0;
                auto data = reinterpret_cast<char *>(&coords);
                memcpy(coords_data + 1, data, 8);
                memcpy(coords_data + 9, &emit_count, 4);

                unsigned char begin_color[3] = { static_cast<unsigned char>(255 * particle_begin_color.x),
                                                 static_cast<unsigned char>(255 * particle_begin_color.y),
                                                 static_cast<unsigned char>(255 * particle_begin_color.z) };
                unsigned char end_color[3] = { static_cast<unsigned char>(255 * particle_end_color.x),
                                               static_cast<unsigned char>(255 * particle_end_color.y),
                                               static_cast<unsigned char>(255 * particle_end_color.z) };
                memcpy(coords_data + 13, begin_color, 3);
                memcpy(coords_data + 16, end_color, 3);
                udp_socket.sendto("25.24.79.180", 7778, coords_data, 20);
            }

            for (int i = 0; i < emit_count; i++) {
                int pc = std::rand() % 10;

                ParticleProps props{};
                props.lifeTime = (pc == 0 ? ((std::rand() / (float)RAND_MAX) * 4.5f) : 1.5f);
                props.position.x = Input::getMousePosition().first - (float)display_w / 2.0f;
                props.position.y = (float)display_h / 2.0f - Input::getMousePosition().second;
                props.begin_color = particle_begin_color;
                props.end_color = particle_end_color;
                props.sizeBegin = 3 + std::rand() % 5;
                props.pol_count = 3 + std::rand() % 3;

                static const double pi = std::acos(-1);

                float angle =  2.0f * pi * std::rand() / (float)RAND_MAX;
                float r = (pc == 0 ? 100.3f : 400.0f) * std::rand() / (float)RAND_MAX;
                float x = r * std::cos(angle);
                float y = r * std::sin(angle);

                props.direction = { x, y };

                particleSystem.emit(props);
            }
        }

        char coords_recv_data[128] = { 0 };
        std::string ip;

        if (udp_socket.recvfrom(ip, 7778, coords_recv_data, 20) > 0) {
            if (reinterpret_cast<unsigned char *>(coords_recv_data)[0] == 0) {
                glm::vec2 coords = {reinterpret_cast<float *>(coords_recv_data + 1)[0],
                                    reinterpret_cast<float *>(coords_recv_data + 1)[1]};

                ImVec4 begin_color = {reinterpret_cast<unsigned char *>(coords_recv_data + 1)[12] / 255.0f,
                                      reinterpret_cast<unsigned char *>(coords_recv_data + 1)[13] / 255.0f,
                                      reinterpret_cast<unsigned char *>(coords_recv_data + 1)[14] / 255.0f,
                                      1.0f};
                ImVec4 end_color = {reinterpret_cast<unsigned char *>(coords_recv_data + 1)[15] / 255.0f,
                                    reinterpret_cast<unsigned char *>(coords_recv_data + 1)[16] / 255.0f,
                                    reinterpret_cast<unsigned char *>(coords_recv_data + 1)[17] / 255.0f,
                                    1.0f};

                for (int i = 0; i < reinterpret_cast<int *>(coords_recv_data + 1)[2]; i++) {
                    int pc = std::rand() % 10;

                    ParticleProps props{};
                    props.lifeTime = (pc == 0 ? ((std::rand() / (float) RAND_MAX) * 4.5f) : 1.5f);
                    props.position.x = coords.x;
                    props.position.y = display_h - coords.y;
                    props.begin_color = begin_color;
                    props.end_color = end_color;
                    props.sizeBegin = 3 + std::rand() % 5;
                    props.pol_count = 3 + std::rand() % 3;

                    static const double pi = std::acos(-1);

                    float angle = 2.0f * pi * std::rand() / (float) RAND_MAX;
                    float r = (rand() % 10 == 0 ? 1.3f : 4.0f) * std::rand() / (float) RAND_MAX;
                    float x = r * std::cos(angle);
                    float y = r * std::sin(angle);

                    props.direction = {x, y};

                    particleSystem.emit(props);
                }
            }
            else {
                spaceShip2.getComponent<core::PositionComponent>().position = {
                        reinterpret_cast<float *>(coords_recv_data + 2)[0],
                        reinterpret_cast<float *>(coords_recv_data + 2)[1]
                };

                spaceShip2.getComponent<core::Transformable>().setRotation(
                        reinterpret_cast<float *>(coords_recv_data + 2)[2]
                        );

                int moveType = *reinterpret_cast<unsigned char *>(coords_recv_data + 1);

                ImVec4 c1, c2;
                bool boost = moveType == 2;

                if (moveType == 1) {
                    c1 = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
                    c2 = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
                }
                else if (moveType == 2) {
                    c1 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    c2 = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
                }

                if (moveType != 0) {
                    for (int i = 0; i < (boost ? 20 : 5); i++) {
                        int pc = std::rand() % 10;

                        ParticleProps props{};
                        props.lifeTime = (pc == 0 ? ((std::rand() / (float) RAND_MAX) * 4.5f) : 2.0f);
                        props.position.x = spaceShip2.getComponent<core::PositionComponent>().position.x;
                        props.position.y = spaceShip2.getComponent<core::PositionComponent>().position.y;
                        props.begin_color = boost ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) :
                                            ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
                        props.end_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
                        props.sizeBegin = 3 + std::rand() % 5;
                        props.pol_count = 3 + std::rand() % 3;

                        static const double pi = std::acos(-1);

                        float angle = 2.0f * pi * std::rand() / (float) RAND_MAX;
                        float r = (pc == 0 ? 1.3f : 4.0f) * std::rand() / (float) RAND_MAX;
                        float x = r * std::cos(angle);
                        float y = r * std::sin(angle);

                        props.direction = {x, y};

                        particleSystem.emit(props);
                    }
                }

            }
        }

        {
            char send_data[14] = { 0 };
            send_data[0] = (unsigned int)(1);

            if (Input::isKeyPressed(KEY_SPACE)) {
                send_data[1] = 2;
            }
            else if (Input::isKeyPressed(KEY_W)) {
                send_data[1] = 1;
            }
            else {
                send_data[1] = 0;
            }

            reinterpret_cast<float *>(send_data + 2)[0] = spaceShip->getComponent<core::PositionComponent>().position.x;
            reinterpret_cast<float *>(send_data + 2)[1] = spaceShip->getComponent<core::PositionComponent>().position.y;
            reinterpret_cast<float *>(send_data + 2)[2] = spaceShip->getComponent<UserControlComponent>().getAngle();

            udp_socket.sendto("25.24.79.180", 7778, send_data, 14);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;


        if (duration.count() < 0.016666f) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(16.666f - duration.count() * 1000.0f)));
            start = std::chrono::high_resolution_clock::now();
            particleSystem.update(0.016666f);
            particleSystem.render(0.016666f);
            core::EntityManager::getInstance()->update(0.016666f);
            core::EntityManager::getInstance()->draw();
        }
        else {
            start = std::chrono::high_resolution_clock::now();
            particleSystem.update(duration.count());
            particleSystem.render(duration.count());
            core::EntityManager::getInstance()->update(duration.count());
            core::EntityManager::getInstance()->draw();
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    shouldClose = true;
    m_chat_thread.join();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}
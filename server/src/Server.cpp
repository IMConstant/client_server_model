//
// Created by Koko on 29.05.2020.
//

#include <algorithm>
#include <sys/stat.h>
#include "Server.h"


bool operator<(const ConnectionData &a, const ConnectionData &b) {
    return a.socket.native() < b.socket.native();
}


Server::Server() {
    if (WSAStartup(MAKEWORD(2, 2), &m_wsa) == 0) {
        std::cout << "WSA startup success" << std::endl;
    }

    m_socket = TCPSocket();
    m_socket.open("", 7777);

    m_udp_socket = UDPSocket();
    m_udp_socket.open("", 7778);
    m_udp_socket.setBlocking(false);

    listen(m_socket.native(), SOMAXCONN);
}

void Server::init() {

}

void Server::gameStateCycle() {
    std::string ip;
    unsigned int port{};

    char udp_recv_data[20] = {0};

    if (m_udp_socket.recvfrom(ip, port, udp_recv_data, 20) > 0) {
        std::cout << "kavo ne ponyal\n";

        for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++) {
            if (ip != connection->ip) {
                m_udp_socket.sendto(connection->ip, 7779, udp_recv_data, 20);
            }
            else {
                m_states.emplace_back(udp_recv_data);
                m_client_states[connection - m_connections.begin()] = true;
            }
        }
    }

    if (!m_connections.empty() && m_client_states.count() == m_connections.size()) {
        m_client_states.reset();
        std::string state{};

        std::cout << "data received" << std::endl;

        for (auto &_state: m_states) {
            state += _state;
        }

        for (auto &connection: m_connections) {
            m_udp_socket.sendto(connection.ip, 7779, state.c_str(), state.size());
        }
    }
}


void Server::run() {
    while (!shouldStop) {
        gameStateCycle();

        fd_set set;
        FD_ZERO(&set);
        FD_SET(m_socket.native(), &set);


        for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++) {
            FD_SET(connection->socket.native(), &set);
        }

        TCPSocket MaxSocket = (!m_connections.empty() ?
                               std::max_element(m_connections.begin(), m_connections.end())->socket :
                               m_socket);

        auto timeout = TIMEVAL();
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        select(0, &set, nullptr, nullptr, &timeout);

        // chat messages
        for (auto connection = m_connections.begin(); connection != m_connections.end();) {
            if (FD_ISSET(connection->socket.native(), &set)) {
                char data[128] = {0};

                int bytes_received = connection->socket.recv(data, 128);

                if (bytes_received <= 0) {
                    shutdown(connection->socket.native(), SD_BOTH);
                    closesocket(connection->socket.native());

                    std::cout << "Connection " << connection->id << " closed" << std::endl;

                    for (auto other = m_connections.begin(); other != m_connections.end(); other++) {
                        if (other != connection) {
                            other->socket.send("User disconnected");
                        }
                    }

                    connection = m_connections.erase(connection);
                    continue;
                }
                else if (bytes_received > 0) {
                    std::cout << data << std::endl;

                    for (auto &other : m_connections) {
                        other.socket.send(data);
                    }
                }
            }

            connection++;
        }

        // new connection
        if (FD_ISSET(m_socket.native(), &set)) {
            sockaddr_in addr{};
            int len = sizeof(sockaddr_in);

            SOCKET sock = ::accept(m_socket.native(), reinterpret_cast<sockaddr *>(&addr), &len);

            if (sock == INVALID_SOCKET) {
                std::cout << "invalid socket";
                std::cout << WSAGetLastError() << std::endl;
            }

            m_connections.emplace_back(TCPSocket(sock), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

            for (auto & connection : m_connections) {
                connection.socket.send("New user connected!");
            }

            std::cout << "New socket connected!" << std::endl;
        }
    }
}
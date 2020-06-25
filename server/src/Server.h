//
// Created by Koko on 29.05.2020.
//

#ifndef CXX17TESTS_SERVER_H
#define CXX17TESTS_SERVER_H

#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <bitset>

#include "socket/Socket.h"


struct ConnectionData {
    TCPSocket socket;
    std::string ip;
    unsigned int port;
    unsigned int id;

    ConnectionData() = delete;

    ConnectionData(TCPSocket _socket, std::string _ip, unsigned char _port) :
        socket(_socket), ip(std::move(_ip)), port(_port), id(getUniqueId()) {}

private:
    static unsigned int getUniqueId() {
        static unsigned char _id{0};

        return _id++;
    }
};



class Server {
    TCPSocket m_socket;
    UDPSocket m_udp_socket;
    WSAData m_wsa;

    std::vector<ConnectionData> m_connections;
    std::bitset<32> m_client_states;
    std::vector<std::string> m_states;

    bool shouldStop = false;

public:
    Server();

    void init();

    void run();
    void stop();

    void gameStateCycle();
};


#endif //CXX17TESTS_SERVER_H

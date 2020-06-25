#include <iostream>

#include "Server.h"


std::ostream &operator<<(std::ostream &output, const char *str) {
    output << std::string(str) << std::string(" saset zhopu");

    return output;
}


int main() {
    Server server;
    server.run();

    return 0;
}

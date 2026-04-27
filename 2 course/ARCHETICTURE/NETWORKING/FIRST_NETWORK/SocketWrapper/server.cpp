#include "SocketWrapper.h"
#include <iostream>

int main() {
    Server server;
    try {
        server.run(1234);
    } catch (const std::exception &e) {
        std::cout << "[Server] Error: " << e.what() << std::endl;
    }
    return 0;
}
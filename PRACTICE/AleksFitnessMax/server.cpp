#include "SocketWrapper.h"
#include <iostream>

int main() {
    Server server;
    const int PORT = 12345;
    
    try {
        server.run(PORT);
    } catch (const std::exception& e) {
        std::cout << "[Server] Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
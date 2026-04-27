#include "SocketWrapper.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Server server;
    bool async_mode = false;
    
    if (argc > 1 && std::string(argv[1]) == "--async") {
        async_mode = true;
    }
    
    try {
        server.run(1234, async_mode);
    } catch (const std::exception &e) {
        std::cout << "[Server] Error: " << e.what() << std::endl;
    }
    return 0;
}
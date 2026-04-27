#include "SocketWrapper.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Client client;
    std::string host = "127.0.0.1";
    
    if (argc > 1) {
        host = argv[1];
    }
    
    try {
        client.run(host, 1234);
    } catch (const std::exception& e) {
        std::cout << "[Client] Error: " << e.what() << std::endl;
    }
    return 0;
}
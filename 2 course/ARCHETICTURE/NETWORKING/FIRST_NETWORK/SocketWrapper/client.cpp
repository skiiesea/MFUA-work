#include "SocketWrapper.h"
#include <iostream>

int main() {
    Client client;
    std::string host;
    std::cout << "Enter server IP: ";
    std::getline(std::cin, host);
    try {
        client.run(host, 1234);
    } catch (const std::exception& e) {
        std::cout << "[Client] Error: " << e.what() << std::endl;
    }
    return 0;
}
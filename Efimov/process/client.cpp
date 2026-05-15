#include "SocketWrapper.h"
#include <iostream>
#include <string>


int main() {
    Client client;
    std::string host;
    std::cout << "Enter server IP: ";
    std::getline(std::cin, host);
    if (host.empty()) {
        host = "127.0.0.1";
    }
    try {
        client.run(host, 12345);
    } 
    catch (const std::exception& e) {
        std::cout << "[Client] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

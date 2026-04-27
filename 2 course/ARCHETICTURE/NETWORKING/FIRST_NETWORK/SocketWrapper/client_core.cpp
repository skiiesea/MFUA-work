#include "SocketWrapper.h"
#include <iostream>

void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;

    std::cout << "[Client] Connecting..." << std::endl;
    client.connect(ip_address, port);
    std::cout << "[Client] Connected to server!" << std::endl;

    while(true) {
        std::string message;
        std::getline(std::cin, message);
        if (message.empty()) continue;
        client.send(message);
        std::string response = client.receive(1024);
        std::cout << "[Client] Server response: " << response << std::endl;
    }
}
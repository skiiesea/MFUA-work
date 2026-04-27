#include "SocketWrapper.h"
#include <iostream>
#include <chrono>

void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;
    
    std::cout << "[Client] Connecting to " << ip_address << ":" << port << "..." << std::endl;
    client.connect(ip_address, port);
    std::cout << "[Client] Connected to server!" << std::endl;
    
    auto start = std::chrono::steady_clock::now();
    
    client.send("GET_TIME\n");
    
    try {
        std::string response = client.receive_line();
        auto end = std::chrono::steady_clock::now();
        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        std::cout << "Response: " << response << std::endl;
        std::cout << "Delay: " << delay << " ms" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "[Client] Error receiving response: " << e.what() << std::endl;
    }
}
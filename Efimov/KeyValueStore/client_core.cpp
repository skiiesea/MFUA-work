#include "SocketWrapper.h"
#include <iostream>
#include <chrono>

void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;
    
    std::cout << "[Client] Connecting to " << ip_address << ":" << port << "..." << std::endl;
    client.connect(ip_address, port);
    std::cout << "[Client] Connected to KV Store Server!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  SET <key> <value>  - Store value" << std::endl;
    std::cout << "  GET <key>          - Retrieve value" << std::endl;
    std::cout << "  DEL <key>          - Delete key" << std::endl;
    std::cout << "  QUIT               - Exit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);
        
        if (command.empty()) continue;
        
        auto start = std::chrono::steady_clock::now();
        
        try {
            client.send(command + "\n");
            std::string response = client.receive_line();
            auto end = std::chrono::steady_clock::now();
            auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            if (response == "NOT_FOUND") {
                std::cout << "[MISS] " << response << " (Delay: " << delay << " ms)" << std::endl;
            } else if (response.find("ERROR:") == 0) {
                std::cout << "[FAIL] " << response << " (Delay: " << delay << " ms)" << std::endl;
            } else if (response == "BYE") {
                std::cout << "Server closed connection (Delay: " << delay << " ms)" << std::endl;
                break;
            } else {
                std::cout << response << " (Delay: " << delay << " ms)" << std::endl;
            }
            
            if (command == "QUIT") {
                break;
            }
            
        } catch (const NetworkException& e) {
            std::cout << "[Client] Network error: " << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cout << "[Client] Error: " << e.what() << std::endl;
            break;
        }
    }
}
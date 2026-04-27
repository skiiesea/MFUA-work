#include "SocketWrapper.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void test_client(int client_id, const std::string& host, int port) {
    try {
        WSAInitializer wsa;
        ClientSocket client;
        
        client.connect(host, port);
        
        auto start = std::chrono::steady_clock::now();
        client.send("GET_TIME\n");
        
        std::string response = client.receive_line();
        auto end = std::chrono::steady_clock::now();
        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        std::cout << "Client " << client_id << " | Response: " << response 
                  << " | Delay: " << delay << " ms" << std::endl;
                  
    } catch (const std::exception& e) {
        std::cout << "Client " << client_id << " Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int num_clients = 5;
    if (argc > 1) {
        num_clients = std::atoi(argv[1]);
    }
    
    std::string host = "127.0.0.1";
    
    std::cout << "Starting " << num_clients << " concurrent clients..." << std::endl;
    
    std::vector<std::thread> threads;
    auto total_start = std::chrono::steady_clock::now();
    
    for (int i = 0; i < num_clients; i++) {
        threads.emplace_back(test_client, i, host, 1234);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto total_end = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();
    
    std::cout << "\nAll " << num_clients << " clients completed in " << total_time << " ms" << std::endl;
    
    return 0;
}
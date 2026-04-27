#include "SocketWrapper.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
#include <vector>

class Logger {
private:
    std::ofstream log_file;
    std::mutex mtx;
    
public:
    Logger() {
        log_file.open("server.log", std::ios::app);
    }
    
    void log(const std::string& client_ip, const std::string& request, const std::string& status) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        log_file << ss.str() << " | " << client_ip << " | Request: " << request << " | Status: " << status << std::endl;
    }
};

class TimeCache {
private:
    std::string cached_time;
    std::mutex mtx;
    std::atomic<bool> running{true};
    std::thread updater_thread;
    
    std::string get_current_time_string() {
        auto now = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "TIME: %Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
public:
    TimeCache() {
        cached_time = get_current_time_string();
        updater_thread = std::thread([this]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                if (running) {
                    std::lock_guard<std::mutex> lock(mtx);
                    cached_time = get_current_time_string();
                }
            }
        });
    }
    
    ~TimeCache() {
        running = false;
        if (updater_thread.joinable()) {
            updater_thread.join();
        }
    }
    
    std::string get_time() {
        std::lock_guard<std::mutex> lock(mtx);
        return cached_time;
    }
};

void handle_client(std::unique_ptr<Socket> client, const std::string& client_ip, Logger& logger, TimeCache& time_cache) {
    try {
        std::string request = client->receive_line();
        
        std::string response;
        std::string status;
        
        if (request == "GET_TIME") {
            response = time_cache.get_time() + "\n";
            status = "OK";
        } else {
            response = "ERROR\n";
            status = "ERROR";
        }
        
        client->send(response);
        logger.log(client_ip, "GET_TIME", status);
        
        std::cout << "[Server] Client " << client_ip << " - " << status << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "[Server] Error handling client " << client_ip << ": " << e.what() << std::endl;
    }
}

void Server::run(int port, bool async_mode) {
    WSAInitializer wsa;
    ServerSocket server(port);
    std::cout << "Server started on port " << port << "..." << std::endl;
    
    if (async_mode) {
        std::cout << "Running in ASYNC mode (multi-threaded with time cache)" << std::endl;
    } else {
        std::cout << "Running in BASIC mode (single-threaded)" << std::endl;
    }
    
    Logger logger;
    TimeCache time_cache;
    
    while (true) {
        auto client = server.accept();
        
        sockaddr_in addr;
        int addr_len = sizeof(addr);
        getpeername(client->get_handle(), (sockaddr*)&addr, &addr_len);
        std::string client_ip = inet_ntoa(addr.sin_addr);
        
        std::cout << "[Server] Client connected from " << client_ip << std::endl;
        
        if (async_mode) {
            std::thread client_thread(handle_client, std::move(client), client_ip, std::ref(logger), std::ref(time_cache));
            client_thread.detach();
        } else {
            handle_client(std::move(client), client_ip, logger, time_cache);
            std::cout << "Client disconnected" << std::endl;
        }
    }
}
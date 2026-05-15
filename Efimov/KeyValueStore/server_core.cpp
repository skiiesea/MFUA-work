#include "SocketWrapper.h"
#include <iostream>
#include <map>
#include <shared_mutex>
#include <fstream>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <mutex>

const int PORT = 12345;
std::atomic<bool> running{true};

class KVStore {
private:
    std::map<std::string, std::string> data;
    mutable std::shared_mutex mtx;
    
public:
    std::string set(const std::string& key, const std::string& value) {
        if (key.empty() || key.find(' ') != std::string::npos) {
            return "ERROR: invalid_key";
        }
        
        std::unique_lock<std::shared_mutex> lock(mtx);
        data[key] = value;
        return "OK";
    }
    
    std::string get(const std::string& key) {
        if (key.empty() || key.find(' ') != std::string::npos) {
            return "ERROR: invalid_key";
        }
        
        std::shared_lock<std::shared_mutex> lock(mtx);
        
        if (data.empty()) {
            return "ERROR: storage_empty";
        }
        
        auto it = data.find(key);
        if (it != data.end()) {
            return "VALUE: " + it->second;
        }
        return "NOT_FOUND";
    }
    
    std::string del(const std::string& key) {
        if (key.empty() || key.find(' ') != std::string::npos) {
            return "ERROR: invalid_key";
        }
        
        std::unique_lock<std::shared_mutex> lock(mtx);
        
        if (data.empty()) {
            return "ERROR: storage_empty";
        }
        
        auto it = data.find(key);
        if (it != data.end()) {
            data.erase(it);
            return "DELETED";
        }
        return "NOT_FOUND";
    }
    
    void dump_to_file(const std::string& filename) {
        std::shared_lock<std::shared_mutex> lock(mtx);
        std::ofstream file(filename);
        
        for (const auto& [key, value] : data) {
            file << key << ":" << value << "\n";
        }
        
        std::cout << "[Store] Dumped " << data.size() << " entries to " << filename << std::endl;
    }
    
    void load_from_file(const std::string& filename) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        std::ifstream file(filename);
        
        if (!file.is_open()) return;
        
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                data[key] = value;
            }
        }
        
        std::cout << "[Store] Loaded " << data.size() << " entries from " << filename << std::endl;
    }
    
    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return data.size();
    }
};

class Logger {
private:
    std::ofstream log_file;
    std::mutex mtx;
    
public:
    Logger() {
        log_file.open("server_kv.log", std::ios::app);
    }
    
    void log(const std::string& client_ip, const std::string& command, const std::string& result) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::time(nullptr);
        log_file << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
                 << " | " << client_ip << " | Command: " << command 
                 << " | Result: " << result << std::endl;
        log_file.flush();
    }
};

KVStore store;
Logger logger;

void periodic_dump() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (running) {
            store.dump_to_file("store_dump.txt");
        }
    }
}

std::string process_command(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::string command;
    iss >> command;
    
    if (command == "SET") {
        std::string key, value;
        iss >> key;
        std::getline(iss, value);
        if (!value.empty() && value[0] == ' ') {
            value = value.substr(1);
        }
        
        if (key.empty()) {
            return "ERROR: invalid_key";
        }
        
        return store.set(key, value);
        
    } else if (command == "GET") {
        std::string key;
        iss >> key;
        
        if (key.empty()) {
            return "ERROR: invalid_key";
        }
        
        return store.get(key);
        
    } else if (command == "DEL") {
        std::string key;
        iss >> key;
        
        if (key.empty()) {
            return "ERROR: invalid_key";
        }
        
        return store.del(key);
        
    } else if (command == "QUIT") {
        return "QUIT";
    }
    
    return "ERROR: unknown_command";
}

void handle_client(std::unique_ptr<Socket> client, const std::string& client_ip, bool async_mode) {
    try {
        while (true) {
            std::string request = client->receive_line();
            
            if (request.empty()) {
                std::cout << "[Server] Client " << client_ip << " disconnected" << std::endl;
                break;
            }
            
            std::cout << "[Server] Received from " << client_ip << ": " << request << std::endl;
            
            std::string response = process_command(request);
            
            if (response == "QUIT") {
                client->send("BYE\n");
                std::cout << "[Server] Client " << client_ip << " quit" << std::endl;
                logger.log(client_ip, request, "BYE");
                break;
            }
            
            client->send(response + "\n");
            logger.log(client_ip, request, response);
            
            std::cout << "[Server] Sent to " << client_ip << ": " << response << std::endl;
        }
    } catch (const NetworkException& e) {
        std::cout << "[Server] Network error with " << client_ip << ": " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[Server] Error with " << client_ip << ": " << e.what() << std::endl;
    }
}

void Server::run(int port) {
    WSAInitializer wsa;
    ServerSocket server(port);

    store.load_from_file("store_dump.txt");
    
    std::cout << "KV Store Server started on port " << port << std::endl;
    std::cout << "Loaded " << store.size() << " entries from dump file" << std::endl;

    std::cout << "Select mode:" << std::endl;
    std::cout << "1. Basic (single-threaded)" << std::endl;
    std::cout << "2. Async (multi-threaded with shared_mutex)" << std::endl;
    std::cout << "Enter choice (1 or 2): ";
    
    int choice;
    std::cin >> choice;
    bool async_mode = (choice == 2);
    
    if (async_mode) {
        std::cout << "Running in ASYNC mode (periodic dump every 5s)" << std::endl;
        std::thread dump_thread(periodic_dump);
        dump_thread.detach();
    } else {
        std::cout << "Running in BASIC mode" << std::endl;
    }
    
    while (running) {
        auto client = server.accept();
        
        sockaddr_in addr;
        int addr_len = sizeof(addr);
        getpeername(client->get_handle(), (sockaddr*)&addr, &addr_len);
        std::string client_ip = inet_ntoa(addr.sin_addr);
        
        std::cout << "[Server] Client connected from " << client_ip << std::endl;
        
        if (async_mode) {
            std::thread client_thread(handle_client, std::move(client), client_ip, true);
            client_thread.detach();
        } else {
            handle_client(std::move(client), client_ip, false);
        }
    }

    store.dump_to_file("store_dump.txt");
    std::cout << "[Server] Shutting down..." << std::endl;
}
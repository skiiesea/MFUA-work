#include <atomic>
#include "SocketWrapper.h"
#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

const int PORT = 12345;
std::atomic<bool> running{true};

struct ClientInfo {
    std::string nickname;
    std::unique_ptr<Socket> socket;
    bool authenticated;
    SOCKET raw_socket;
};

std::map<SOCKET, ClientInfo> clients;
std::mutex clients_mutex;

// Простое XOR шифрование (для демонстрации)
std::string encrypt_decrypt(const std::string& input, char key = 0x5A) {
    std::string result = input;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

void broadcast(const std::string& message, SOCKET exclude = INVALID_SOCKET) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    std::string encrypted = encrypt_decrypt(message);
    
    for (auto& [sock, info] : clients) {
        if (sock != exclude && info.authenticated) {
            try {
                info.socket->send(encrypted + "\n");
            } catch (...) {
                // Игнорируем ошибки отправки
            }
        }
    }
}

void update_user_list() {
    std::string user_list = "/users ";
    for (auto& [sock, info] : clients) {
        if (info.authenticated) {
            user_list += info.nickname + " ";
        }
    }
    
    std::lock_guard<std::mutex> lock(clients_mutex);
    std::string encrypted = encrypt_decrypt(user_list);
    
    for (auto& [sock, info] : clients) {
        if (info.authenticated) {
            try {
                info.socket->send(encrypted + "\n");
            } catch (...) {}
        }
    }
}

void handle_file_transfer(SOCKET sender_sock, ClientInfo& sender, size_t file_size) {
    sender.socket->send(encrypt_decrypt("READY") + "\n");
    
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    size_t received = 0;
    
    auto start_time = std::chrono::steady_clock::now();
    
    // Получаем файл от отправителя
    while (received < file_size) {
        size_t to_receive = std::min(BUFFER_SIZE, file_size - received);
        size_t bytes = sender.socket->receive(buffer, to_receive);
        if (bytes == 0) break;
        
        received += bytes;
        
        // Пересылаем всем остальным
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& [sock, info] : clients) {
            if (sock != sender_sock && info.authenticated) {
                try {
                    info.socket->send(buffer, bytes);
                } catch (...) {}
            }
        }
    }
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    double speed_mbps = (file_size * 8.0) / (duration.count() / 1000.0) / 1000000.0;
    
    std::cout << "[SERVER] File transfer completed: " << file_size/1024 << " KB" << std::endl;
    std::cout << "[SERVER] Time: " << duration.count() << " ms, Speed: " << speed_mbps << " Mbps" << std::endl;
    
    broadcast("[SERVER] " + sender.nickname + " sent a file (" + std::to_string(file_size/1024) + " KB)");
}

void handle_client(std::unique_ptr<Socket> client_sock, const std::string& ip) {
    SOCKET raw_socket = client_sock->get_handle();
    std::string nickname;
    
    try {
        // Аутентификация
        std::string encrypted_auth = client_sock->receive_line();
        std::string auth_data = encrypt_decrypt(encrypted_auth);
        
        if (auth_data.find("LOGIN:") != 0) {
            client_sock->send(encrypt_decrypt("AUTH_FAIL") + "\n");
            return;
        }
        
        size_t first = auth_data.find(':');
        size_t second = auth_data.find(':', first + 1);
        nickname = auth_data.substr(first + 1, second - first - 1);
        std::string password = auth_data.substr(second + 1);
        
        // Простая проверка (любой пароль подходит, для демо)
        if (nickname.empty()) {
            client_sock->send(encrypt_decrypt("AUTH_FAIL") + "\n");
            return;
        }
        
        // Проверка на уникальность ника
        bool name_taken = false;
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (auto& [sock, info] : clients) {
                if (info.nickname == nickname) {
                    name_taken = true;
                    break;
                }
            }
        }
        
        if (name_taken) {
            client_sock->send(encrypt_decrypt("AUTH_FAIL") + "\n");
            std::cout << "[SERVER] Auth failed for " << nickname << " (name taken)" << std::endl;
            return;
        }
        
        client_sock->send(encrypt_decrypt("AUTH_OK") + "\n");
        
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients[raw_socket] = {
                nickname,
                std::move(client_sock),
                true,
                raw_socket
            };
        }
        
        std::cout << "[SERVER] " << nickname << " joined from " << ip << std::endl;
        broadcast("[SERVER] " + nickname + " joined the chat");
        update_user_list();
        
        // Основной цикл
        while (running) {
            std::string encrypted_msg;
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                if (clients.find(raw_socket) == clients.end()) break;
                encrypted_msg = clients[raw_socket].socket->receive_line();
            }
            
            if (encrypted_msg.empty()) break;
            
            std::string message = encrypt_decrypt(encrypted_msg);
            
            if (message == "/quit") {
                break;
            }
            else if (message == "/users") {
                std::string user_list = "/users ";
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (auto& [sock, info] : clients) {
                    if (info.authenticated) {
                        user_list += info.nickname + " ";
                    }
                }
                clients[raw_socket].socket->send(encrypt_decrypt(user_list) + "\n");
            }
            else if (message.find("/file_start:") == 0) {
                size_t file_size = std::stoull(message.substr(12));
                std::cout << "[SERVER] " << nickname << " sending file (" << file_size/1024 << " KB)" << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    handle_file_transfer(raw_socket, clients[raw_socket], file_size);
                }
            }
            else if (!message.empty()) {
                std::cout << "[" << nickname << "]: " << message << std::endl;
                broadcast(message, raw_socket);
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "[SERVER] Error with " << (nickname.empty() ? ip : nickname) << ": " << e.what() << std::endl;
    }
    
    // Отключение клиента
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = clients.find(raw_socket);
        if (it != clients.end()) {
            std::cout << "[SERVER] " << it->second.nickname << " disconnected" << std::endl;
            broadcast("[SERVER] " + it->second.nickname + " left the chat");
            clients.erase(it);
        }
    }
    update_user_list();
}

void Server::run(int port) {
    WSAInitializer wsa;
    ServerSocket server(port, false);
    
    std::cout << "[SERVER] Chat Server started on port " << port << std::endl;
    std::cout << "[SERVER] Waiting for connections..." << std::endl;
    
    while (running) {
        try {
            auto client = server.accept();
            sockaddr_in addr;
            int addr_len = sizeof(addr);
            getpeername(client->get_handle(), (sockaddr*)&addr, &addr_len);
            std::string ip = inet_ntoa(addr.sin_addr);
            
            std::thread(handle_client, std::move(client), ip).detach();
            
        } catch (const std::exception& e) {
            if (running) {
                std::cout << "[SERVER] Accept error: " << e.what() << std::endl;
            }
        }
    }
}
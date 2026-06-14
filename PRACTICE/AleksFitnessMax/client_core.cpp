#include <atomic>
#include "SocketWrapper.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>

std::atomic<bool> running{true};

// Простое XOR шифрование (для демонстрации)
std::string encrypt_decrypt(const std::string& input, char key = 0x5A) {
    std::string result = input;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

void receive_thread(Socket& sock, const std::string& nickname) {
    while (running) {
        try {
            std::string encrypted = sock.receive_line();
            if (encrypted.empty()) {
                std::cout << "\n[DISCONNECTED] Server closed connection" << std::endl;
                running = false;
                break;
            }
            
            std::string msg = encrypt_decrypt(encrypted);
            
            // Вывод сообщений с цветовой маркировкой
            if (msg.find("[SERVER]") == 0) {
                std::cout << msg << std::endl;
            } else if (msg.find("/users") == 0) {
                std::cout << "=== Online Users ===" << std::endl;
                std::cout << msg.substr(6) << std::endl;
                std::cout << "====================" << std::endl;
            } else {
                std::cout  << msg << std::endl;
            }
            
            std::cout << "[" << nickname << "]: ";
            std::cout.flush();
            
        } catch (const std::exception& e) {
            if (running) {
                std::cout << "\n[ERROR] " << e.what() << std::endl;
            }
            break;
        }
    }
}

void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;
    
    std::cout << "[CLIENT] Connecting to " << ip_address << ":" << port << "..." << std::endl;
    client.connect(ip_address, port);
    std::cout << "[CLIENT] Connected!" << std::endl;
    
    // Аутентификация
    std::string nickname, password;
    std::cout << "=== AUTHENTICATION ===" << std::endl;
    std::cout << "Enter nickname: ";
    std::getline(std::cin, nickname);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    std::string auth_msg = "LOGIN:" + nickname + ":" + password;
    client.send(encrypt_decrypt(auth_msg) + "\n");
    
    std::string response = client.receive_line();
    if (response == "AUTH_FAIL") {
        std::cout << "[CLIENT] Authentication failed!" << std::endl;
        return;
    }
    
    std::cout << "\n[CLIENT] Welcome, " << nickname << "!" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  /users     - Show online users" << std::endl;
    std::cout << "  /file <path> - Send file" << std::endl;
    std::cout << "  /quit      - Exit chat" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "[" << nickname << "]: ";
    
    // Запуск потока для приема сообщений
    std::thread receiver(receive_thread, std::ref(client), nickname);
    
    // Основной поток для отправки
    while (running) {
        std::string message;
        std::getline(std::cin, message);
        
        if (message == "/quit") {
            client.send(encrypt_decrypt("/quit") + "\n");
            running = false;
            break;
        }
        else if (message == "/users") {
            client.send(encrypt_decrypt("/users") + "\n");
            continue;
        }
        else if (message.find("/file ") == 0) {
            std::string filepath = message.substr(6);
            
            // Открываем файл
            std::ifstream file(filepath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cout << "[CLIENT] Cannot open file: " << filepath << std::endl;
                std::cout << "[" << nickname << "]: ";
                continue;
            }
            
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            // Отправляем команду файла
            std::string file_info = "/file_start:" + std::to_string(file_size);
            client.send(encrypt_decrypt(file_info) + "\n");
            
            // Ждем подтверждения
            std::string ack = client.receive_line();
            
            // Отправляем файл частями
            const size_t BUFFER_SIZE = 4096;
            char buffer[BUFFER_SIZE];
            size_t total_sent = 0;
            
            auto start_time = std::chrono::steady_clock::now();
            
            while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
                client.send(buffer, file.gcount());
                total_sent += file.gcount();
                
                // Прогресс
                if (file_size > 1024*1024) { // Только для больших файлов
                    int percent = (total_sent * 100) / file_size;
                    std::cout << "\r[SENDING] " << percent << "% (" << total_sent/1024 << " KB)";
                    std::cout.flush();
                }
            }
            
            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            double speed_mbps = (file_size * 8.0) / (duration.count() / 1000.0) / 1000000.0;
            
            std::cout << "\n[CLIENT] File sent! Size: " << file_size/1024 << " KB" << std::endl;
            std::cout << "[CLIENT] Time: " << duration.count() << " ms, Speed: " << speed_mbps << " Mbps" << std::endl;
            
            file.close();
            std::cout << "[" << nickname << "]: ";
            continue;
        }
        else if (!message.empty()) {
            // Обычное текстовое сообщение
            std::string formatted = "[" + nickname + "]: " + message;
            client.send(encrypt_decrypt(formatted) + "\n");
        }
        else {
            std::cout << "[" << nickname << "]: ";
        }
    }
    
    receiver.join();
    std::cout << "[CLIENT] Goodbye!" << std::endl;
}
#include "SocketWrapper.h"
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iomanip>



std::atomic<long long> g_cmd_start_time{0};
std::atomic<bool> g_is_csv_command{false};


void print_latency_if_active() {
    long long start_ms = g_cmd_start_time.exchange(0);
    if (start_ms > 0) {
        auto end = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::high_resolution_clock::time_point(std::chrono::milliseconds(start_ms));
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "(Round-trip latency: " << duration << " ms)" << std::endl;
    }
}


void receive_handler(ClientSocket* client, std::atomic<bool>& is_running) {
    std::string response_buffer;
    bool processing_csv = false;
    try {
        while (is_running) {
            std::string chunk = client->receive(1024);
            if (chunk.empty()) {
                std::cout << "\n[Client] Connection closed" << std::endl;
                is_running = false;
                break;
            }
            response_buffer += chunk;
            size_t pos;
            while ((pos = response_buffer.find('\n')) != std::string::npos) {
                std::string line = response_buffer.substr(0, pos);
                response_buffer.erase(0, pos + 1);

                if (line.empty()) continue;
                if (line.back() == '\r') line.pop_back();
                if (line == "PING") {
                    client->send("PONG\n");
                    continue;
                }

                if (line == "CSV_LIST") {
                    processing_csv = true;
                    std::cout << "\n" << std::endl;
                    std::cout << "| Process                     | PID    | CPU%      |" << std::endl;
                    continue;
                }

                if (processing_csv) {
                    if (line == "RUNNING" || line.rfind("ERROR", 0) == 0) {
                        processing_csv = false;
                        print_latency_if_active();
                        continue;
                    } else {
                        std::stringstream ss(line);
                        std::string name, pid, cpu;
                        if (std::getline(ss, name, ',') && std::getline(ss, pid, ',') && std::getline(ss, cpu, ',')) {
                            std::cout << "| " << std::left << std::setw(27) << name 
                                      << " | " << std::setw(6) << pid 
                                      << " | " << std::setw(5) << cpu << "% |" << std::endl;
                        }
                        continue;
                    }
                }

                if (line == "NOT_FOUND") {
                    std::cout << "[MISS] Key does not exist in storage" << std::endl;
                } 
                else if (line.rfind("ERROR:", 0) == 0 || line == "ERROR") {
                    std::cout << "[FAIL] " << line << std::endl; 
                } 
                else if (line.rfind("RUNNING PID:", 0) == 0 || line == "NOT_RUNNING") {
                    std::cout << "[Client] Process Status: " << line << std::endl;
                }
                else if (line.rfind("[SYSTEM]", 0) == 0 || line.rfind("[", 0) == 0) {
                    std::cout << ">> " << line << std::endl;
                }
                else {
                    std::cout << "[Client] Server response: " << line << std::endl;
                }

                if (!g_is_csv_command) {
                    print_latency_if_active();
                }
            }
        }
    } catch (...) {
        is_running = false;
    }
}


void Client::run(const std::string& ip_address, int port) {
    WSAInitializer wsa;
    ClientSocket client;
    std::cout << "[Client] Connecting..." << std::endl;
    try {
        client.connect(ip_address, port);
        std::cout << "[Client] Connected to server" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[Client] Error: " << e.what() << std::endl;
        return;
    }
    std::atomic<bool> is_running(true);
    std::thread rx_thread(receive_handler, &client, std::ref(is_running));
    std::string input_message;
    while (is_running && std::getline(std::cin, input_message)) {
        if (input_message.empty()) continue; 
        g_is_csv_command = (input_message == "csv_list" || input_message == "CSV_LIST");
        auto start = std::chrono::high_resolution_clock::now();
        g_cmd_start_time = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        try {
            client.send(input_message + "\n");
            if (input_message == "QUIT") { 
                is_running = false; 
                break; 
            }
        } catch (...) {
            is_running = false;
            break;
        }
    }
    is_running = false;
    client.close();
    if (rx_thread.joinable()) rx_thread.join();
}

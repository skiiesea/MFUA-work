#include "utils.h"
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <iostream>

std::mutex csl_mtx;
std::atomic<int> active_connections{0};
const int MAX_ACTIVE_CONNECTIONS = 12;

void safe_log(const std::string &message) {
    std::lock_guard<std::mutex> lk{csl_mtx};
    std::cout << message << std::endl;
}


std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto timestamp_val = std::chrono::system_clock::to_time_t(now);
    std::tm tm_val;
    localtime_s(&tm_val, &timestamp_val);
    std::ostringstream oss;
    oss << std::put_time(&tm_val, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string get_peer_ip(SOCKET s) {
    sockaddr_storage address;
    socklen_t len = sizeof(address);
    if (getpeername(s, reinterpret_cast<sockaddr*>(&address), &len) == 0) {
        char ip[INET6_ADDRSTRLEN];
        if (address.ss_family == AF_INET)
            inet_ntop(AF_INET, &reinterpret_cast<sockaddr_in*>(&address)->sin_addr, ip, sizeof(ip));
        else
            inet_ntop(AF_INET6, &reinterpret_cast<sockaddr_in6*>(&address)->sin6_addr, ip, sizeof(ip));
        return ip;
    }
    return "unknown";
}

void receive_exact(Socket &socket, void *buffer, size_t size) {
    size_t total = 0;
    char *buf = static_cast<char*>(buffer);
    while (total < size) {
        size_t got = socket.receive(buf + total, size - total);
        if (got == 0) throw NetworkException("Connection issues");
        total += got;
    }
}

void handle_client(std::unique_ptr<Socket> client) {
    ++active_connections;
    int name_len_net = 0;
    receive_exact(*client, &name_len_net, sizeof(name_len_net));
    int name_len = ntohl(name_len_net);
    if (name_len > 1024) throw NetworkException("Filename too long");
    std::string filename(name_len, '\0');
    receive_exact(*client, filename.data(), name_len);
    
    std::string filepath = (std::filesystem::path("data") / filename).string();
    std::string timestamp = get_timestamp();
    std::string client_ip = get_peer_ip(client->get_handle());

    if (std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath)) {
        unsigned long long file_sz = std::filesystem::file_size(filepath);
        std::ifstream file(filepath, std::ios::binary);
        if (!file) throw NetworkException("Cannot open file!");
        unsigned char status = 1;
        client->send(&status, sizeof(status));
        unsigned char size_bytes[8];
        unsigned long long temp = file_sz;
        for (int i = 0; i < 7; ++i) {
            size_bytes[i] = static_cast<unsigned short>(temp & 0xFF);
            temp >>= 8;
        }
        client->send(size_bytes, 8);

        const size_t CHUNK = 8192;
        char buffer[CHUNK];
        while(file.read(buffer, CHUNK)) {
            client->send(buffer, static_cast<size_t>(file.gcount()));
        }
        if (file.gcount() > 0) {
            client->send(buffer, static_cast<size_t>(file.gcount()));
        }
        std::cout << "[" << timestamp << "] File succesful sent: " << filename 
                  << " | Client IP: " << client_ip << std::endl;
    } else {
        unsigned char status = 0;
        client->send(&status, sizeof(status));
        std::cout << "[" << timestamp << "] File not found: " << filename 
                  << " | Client IP: " << client_ip << std::endl;
    }
    --active_connections;
}
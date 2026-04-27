#include "utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#ifdef min
#undef min
#endif

int main() {
    try {
        WSAInitializer wsa;
        std::string server_ip;
        std::cout << "Enter server IP: ";
        std::getline(std::cin, server_ip);
        ClientSocket client;

        std::cout << "[Client] Connecting..." << std::endl;
        client.connect(server_ip, 12345);
        std::cout << "[Client] Connected to server!" << std::endl;
        std::string filename;
        std::cout << "Enter filename: ";
        std::getline(std::cin, filename);
        if (filename.empty()) return 0;
        unsigned long name_len_net = htonl(static_cast<unsigned>(filename.size()));
        client.send(&name_len_net, sizeof(name_len_net));

        client.send(filename.c_str(), filename.size());

        unsigned char status = 0;
        receive_exact(client,&status, sizeof(status));

        if (status == 1) {
            unsigned char size_bytes[8];
            receive_exact(client, size_bytes, 8);
            unsigned long long fsize = 0;
            for (int i = 7; i >= 0; --i)
                fsize = (fsize << 8) | size_bytes[i];
            
            std::ofstream file(filename, std::ios::binary);
            if (!file) throw NetworkException("Cannot create file!");
            const size_t CHUNK = 8192;
            char buffer[CHUNK];
            unsigned long long received = 0;
            while (received < fsize) {
                unsigned long long to_read = std::min(static_cast<unsigned long long>(CHUNK), fsize - received);
                size_t got = client.receive(buffer, static_cast<size_t>(to_read));
                if (got == 0) throw NetworkException("Cannot read file from server");
                file.write(buffer, static_cast<std::streamsize>(got));
                received += got;
            }
            file.close();
            std::cout << "[" << get_timestamp() << "] Succesfully get file: " << filename
                      << " | size: " << fsize << " bytes" << std::endl;
        } else {
            std::cout << "[" << get_timestamp() << "] File not found on server" << std::endl;
        }
    } catch (std::exception &e) {
        std::cout << "Client error: " << e.what() << std::endl;
    }
    return 0;
}
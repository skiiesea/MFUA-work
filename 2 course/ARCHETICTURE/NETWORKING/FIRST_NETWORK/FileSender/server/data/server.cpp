#include "utils.h"
#include <filesystem>
#include <iostream>

int main() {
    try {
        WSAInitializer wsa;
        const int PORT = 12345;
        std::filesystem::create_directory("data");
        ServerSocket server(PORT);
        safe_log("Server is up at port: " + std::to_string(PORT));
        while(true) {
            if (active_connections.load() >= MAX_ACTIVE_CONNECTIONS) {
                safe_log("[WARNING] Max active connections reached. Wait please...");
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
            auto client = server.accept();
            std::thread(handle_client, std::move(client)).detach();
        }

    } catch (std::exception &e) {
        safe_log("Server error: " + std::string(e.what()));
    }
    return 0;
}
#include "SocketWrapper.h"

const int PORT = 12345;
const int TIMEOUT_SECONDS = 3;

std::atomic<int> operation_counter{0};
std::atomic<bool> stats_running{true};

class Logger {
private:
    std::ofstream log_file;
    std::mutex mtx;
    
public:
    Logger() {
        log_file.open("server_calc.log", std::ios::app);
    }
    
    void log(const std::string& client_ip, const std::string& expression, const std::string& result) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::time(nullptr);
        log_file << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
                 << " | " << client_ip << " | Expression: " << expression 
                 << " | Result: " << result << std::endl;
        log_file.flush();
    }
};

double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw std::runtime_error("division_by_zero");
            return a / b;
        default: throw std::runtime_error("invalid_operator");
    }
}

std::string process_expression(const std::string& expression) {
    std::regex pattern(R"(CALC\s+(-?\d+(?:\.\d+)?)\s+([+\-*/])\s+(-?\d+(?:\.\d+)?))");
    std::smatch match;
    
    if (!std::regex_match(expression, match, pattern)) {
        return "ERROR: invalid_syntax";
    }
    
    try {
        double a = std::stod(match[1].str());
        double b = std::stod(match[3].str());
        char op = match[2].str()[0];
        
        double result = calculate(a, b, op);

        std::stringstream ss;
        if (result == std::floor(result)) {
            ss << static_cast<long long>(result);
        } else {
            ss << std::fixed << std::setprecision(6) << result;
            std::string res_str = ss.str();
            res_str.erase(res_str.find_last_not_of('0') + 1, std::string::npos);
            if (res_str.back() == '.') res_str.pop_back();
            return "RESULT: " + res_str;
        }
        return "RESULT: " + ss.str();
        
    } catch (const std::runtime_error& e) {
        return "ERROR: " + std::string(e.what());
    } catch (...) {
        return "ERROR: invalid_syntax";
    }
}

std::string process_with_timeout(const std::string& expression) {
    auto future = std::async(std::launch::async, [&expression]() {
        return process_expression(expression);
    });
    
    if (future.wait_for(std::chrono::seconds(TIMEOUT_SECONDS)) == std::future_status::timeout) {
        return "ERROR: timeout";
    }
    
    return future.get();
}

void print_statistics() {
    while (stats_running) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "[STATS] Total operations completed: " << operation_counter.load() << std::endl;
    }
}

void handle_client(std::unique_ptr<Socket> client, const std::string& client_ip, Logger& logger, bool async_mode) {
    try {
        while (true) {
            std::string request = client->receive_line();
            
            if (request.empty()) {
                std::cout << "[Server] Client " << client_ip << " disconnected" << std::endl;
                break;
            }
            
            std::cout << "[Server] Received from " << client_ip << ": " << request << std::endl;
            
            std::string response;
            
            if (async_mode) {
                response = process_with_timeout(request);
                if (response.find("RESULT:") == 0) {
                    operation_counter++;
                }
            } else {
                response = process_expression(request);
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
    std::cout << "Calculator Server started on port " << port << std::endl;
    std::cout << "1. Basic" << std::endl;
    std::cout << "2. Async" << std::endl;
    std::cout << "Enter choice (1 or 2): ";
    
    int choice;
    std::cin >> choice;
    bool async_mode = (choice == 2);
    
    if (async_mode) {
        std::cout << "Running in ASYNC mode" << std::endl;
        std::thread stats_thread(print_statistics);
        stats_thread.detach();
    } else {
        std::cout << "Running in BASIC mode" << std::endl;
    }
    
    Logger logger;
    
    while (true) {
        auto client = server.accept();
        
        sockaddr_in addr;
        int addr_len = sizeof(addr);
        getpeername(client->get_handle(), (sockaddr*)&addr, &addr_len);
        std::string client_ip = inet_ntoa(addr.sin_addr);
        
        std::cout << "[Server] Client connected from " << client_ip << std::endl;
        
        if (async_mode) {
            std::thread client_thread(handle_client, std::move(client), client_ip, std::ref(logger), true);
            client_thread.detach();
        } else {
            handle_client(std::move(client), client_ip, logger, false);
        }
    }
}
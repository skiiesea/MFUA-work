#ifndef SOCKWRAPPER_H
#define SOCKWRAPPER_H

#include <WinSock2.h>
#include <string>
#include <WS2tcpip.h>
#include <stdexcept>
#include <memory>
#include <chrono>

#pragma comment(lib, "ws2_32.lib");

class NetworkException : public std::runtime_error {
public:
    explicit NetworkException(const std::string &message);
};

class WSAInitializer {
public:
    WSAInitializer();
    ~WSAInitializer();
    WSAInitializer(const WSAInitializer&) = delete;
    WSAInitializer& operator=(const WSAInitializer&) = delete;
};

class Socket {
protected:
    SOCKET socket_;
public:
    explicit Socket(SOCKET socket);
    Socket(const Socket&) = delete;
    Socket(Socket&&) noexcept;
    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&&) noexcept;
    virtual ~Socket(); 
    void close();
    bool is_valid() const;
    SOCKET get_handle() const;
    void send(const std::string&);
    void send(const void*, size_t);
    size_t receive(void* buffer, size_t);
    std::string receive(size_t);
    std::string receive_line(); // Новый метод для получения строки до \n
};

class ClientSocket : public Socket {
public:
    ClientSocket();
    void connect(const std::string& host, int port);
};

class ServerSocket : public Socket {
public:
    ServerSocket(int port, bool local_host_only = false);
    std::unique_ptr<Socket> accept();
};

class Server final {
public:
    void run(int port, bool async_mode = false);
};

class Client final {
public:
    void run(const std::string& ip_address, int port);
};

#endif
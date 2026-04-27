#pragma once
#include "../SocketWrapper/SocketWrapper.h"
#include <string>
#include <chrono>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>

extern std::mutex csl_mtx;
extern std::atomic<int> active_connections;
extern const int MAX_ACTIVE_CONNECTIONS;

void safe_log(const std::string &message);

std::string get_timestamp();

std::string get_peer_ip(SOCKET s);

void receive_exact(Socket &socket, void *buffer, size_t size);

void handle_client(std::unique_ptr<Socket> client);
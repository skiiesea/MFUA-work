#include "SocketWrapper.h"

NetworkException::NetworkException(const std::string &message)
    : std::runtime_error(message + " (Error: " + std::to_string(WSAGetLastError()) + ")") {}

WSAInitializer::WSAInitializer() {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        throw NetworkException("WSAStartup failed");
}

WSAInitializer::~WSAInitializer() {
    WSACleanup();
}

//     ____  _  __ _  _  _____ ____  _____ ____ 
//    / ___\/ |/ // \/ \/  __// ___\/  __//  _ \
//    |    \|   / | || ||  \  |    \|  \  | / \|
//    \___ ||   \ | || ||  /_ \___ ||  /_ | |-||
//    \____/\_|\_\\_/\_/\____\\____/\____\\_/ \|
//                                              
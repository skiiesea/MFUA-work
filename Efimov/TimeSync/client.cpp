#include "SocketWrapper.h"
#include <iostream>

int main() {
    Client client;
    std::string host;
    std::cout << "Enter server IP: ";
    std::getline(std::cin, host);
    try {
        client.run(host, 12345);
    } catch (const std::exception& e) {
        std::cout << "[Client] Error: " << e.what() << std::endl;
    }
    return 0;
}

//     ____  _  __ _  _  _____ ____  _____ ____ 
//    / ___\/ |/ // \/ \/  __// ___\/  __//  _ \
//    |    \|   / | || ||  \  |    \|  \  | / \|
//    \___ ||   \ | || ||  /_ \___ ||  /_ | |-||
//    \____/\_|\_\\_/\_/\____\\____/\____\\_/ \|
//                                              
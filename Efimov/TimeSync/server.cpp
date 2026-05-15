#include "SocketWrapper.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Server server;
    try {
        server.run(12345);
    } catch (const std::exception &e) {
        std::cout << "[Server] Error: " << e.what() << std::endl;
    }
    return 0;
}


//     ______     __  __     __     __     ______     ______     ______     ______    
//    /\  ___\   /\ \/ /    /\ \   /\ \   /\  ___\   /\  ___\   /\  ___\   /\  __ \   
//    \ \___  \  \ \  _"-.  \ \ \  \ \ \  \ \  __\   \ \___  \  \ \  __\   \ \  __ \  
//     \/\_____\  \ \_\ \_\  \ \_\  \ \_\  \ \_____\  \/\_____\  \ \_____\  \ \_\ \_\ 
//      \/_____/   \/_/\/_/   \/_/   \/_/   \/_____/   \/_____/   \/_____/   \/_/\/_/ 
//                                                                                    


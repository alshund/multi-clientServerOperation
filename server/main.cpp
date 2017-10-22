#include "server/Server.h"

#pragma comment(lib, "ws_32.lib")

using namespace std;

BOOL WINAPI ConsoleHandler(DWORD);


int main() {
//    server->start();
 //   std::cout << "kek" << std::endl;
    auto& server = Server::getInstance();
    signal(SIGINT, Server::interruption_handler);

    server.start();

    return 0;
}

#include <iostream>
#include <ws2tcpip.h>
#include <thread>
#include <functional>
#include <sstream>
#include <vector>
#include <mutex>

#include "server/Server.h"

#pragma comment(lib, "ws_32.lib")

using namespace std;

BOOL WINAPI ConsoleHandler(DWORD);

void my_handler(int param) {
    //TODO: add server distructor
//    delete server;
    std::cout << "kek" << std::endl;
    system("Pause");
    exit(0);
}

int main() {
//    server->start();
    auto& server = Server::getInstance();
    signal(SIGINT, my_handler);

    server.start();

    return 0;
}

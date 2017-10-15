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

    signal(SIGINT, my_handler);

    auto& server = Server::getInstance();

    server.start();

    return 0;
}

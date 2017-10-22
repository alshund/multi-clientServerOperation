//
// Created by shund on 15.10.2017.
//

#include <inaddr.h>

#include "Server.h"
#include "Connection.h"

#pragma comment(lib, "ws_32.lib")

Server::Server() {

    adjustWsaData();
    adjustListeningSocket();
    bindSocketHint();
}

Server::~Server() {

    closesocket(listeningSocket);
    WSACleanup();
}

void Server::adjustWsaData() {

    int wsaStartup = WSAStartup(winsSockVersion, &winSockData);
    if (wsaStartup != 0) {
        std::cerr << "Can't initialize winSock, Err #" << WSAGetLastError() << std::endl;
    }
}

void Server::adjustListeningSocket() {

    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket == INVALID_SOCKET) {
        std::cerr << "Can't create a listening socket, Err #" << WSAGetLastError() << std::endl;
    }
}

void Server::bindSocketHint() {

    sockaddr_in listeningSocketHint;
    listeningSocketHint.sin_family = AF_INET;
    listeningSocketHint.sin_port = htons(PORT);
    listeningSocketHint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(listeningSocket, (sockaddr *) &listeningSocketHint, sizeof(listeningSocketHint));
}

Server& Server::getInstance() {

    static Server server;
    return server;
}
void Server::interruption_handler(int param) {
//    delete getInstance();
//    std::cout << "kek" << std::endl;
    system("Pause");
    exit(0);
}
void Server::start() {

    listen(listeningSocket, SOMAXCONN);
    while (true) {
        sockaddr_in clientSocketHint;
        int clientSocketHintSize = sizeof(clientSocketHint);
        SOCKET  clientSocket = accept(listeningSocket, (sockaddr *) &clientSocketHint, &clientSocketHintSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client could't connect, Err #" << WSAGetLastError() << std::endl;
        }

        Connection *connection = new Connection(clientSocket, clientSocketHint.sin_addr.S_un.S_addr, buffer);
        std::thread connectionThread (&Connection::clientProcessing, std::ref(connection));
        connectionThread.detach();
    }
}
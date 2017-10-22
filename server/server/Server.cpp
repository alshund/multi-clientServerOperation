//
// Created by shund on 15.10.2017.
//

#include <inaddr.h>
#include <direct.h>
#include <fstream>
#include <sstream>

#include "Server.h"

#pragma comment(lib, "ws_32.lib")

Server::Server() {

    adjustWsaData();
    adjustListeningSocket();
    bindSocketHint();
}

Server::~Server() {

//    closesocket(listeningSocket);
//    WSACleanup();
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
    getInstance().stopServer();
    system("Pause");
    exit(0);
}

void Server::stopServer(){
    isActive = false;
    shutDownAllConnections();
    dumpLog();
}

void Server::dumpLog() {
    char current_work_dir[FILENAME_MAX];
    _getcwd(current_work_dir, sizeof(current_work_dir));

    std::stringstream stringStream;
    stringStream << current_work_dir << "\\tmp"<<"\\log.txt";
    std::string filePath = stringStream.str();


    std::ofstream fout;
    fout.open(filePath);
    for (int i; i<buffer.size(); i++){
        fout<<buffer[i];
    }
    fout.close();
    std::cout<<filePath<<std::endl;

}

void Server::start() {
    listen(listeningSocket, SOMAXCONN);
    isActive = true;
    while (isActive) {
        sockaddr_in clientSocketHint;
        int clientSocketHintSize = sizeof(clientSocketHint);
        SOCKET  clientSocket = accept(listeningSocket, (sockaddr *) &clientSocketHint, &clientSocketHintSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client could't connect, Err #" << WSAGetLastError() << std::endl;
        }

        Connection *connection = new Connection(clientSocket, clientSocketHint.sin_addr.S_un.S_addr);
        std::thread connectionThread (&Connection::clientProcessing, std::ref(connection));
        connections.push_back(connection);
        connectionThread.detach();    }
}

void Server::addMessage(std::string message) {
    mutex.lock();
    buffer.push_back(message);
    mutex.unlock();
}

void Server::deleteConnection(Connection *connection) {
    mutex.lock();
    for (std::vector<Connection*>::iterator eraseIterator = connections.begin(); eraseIterator < connections.end(); eraseIterator++) {
        if (*eraseIterator == connection) {
            connections.erase(eraseIterator);
        }
    }
    mutex.unlock();
}

void Server::shutDownAllConnections() {
    mutex.lock();
    for (int connectionIndex = 0; connectionIndex < connections.size(); connectionIndex++) {
        connections[connectionIndex]->closeSocket();
    }
    mutex.unlock();
}
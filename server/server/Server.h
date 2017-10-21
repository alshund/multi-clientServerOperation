//
// Created by shund on 15.10.2017.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <ws2tcpip.h>

#include <iostream>
#include <vector>
#include <thread>
#include "Connection.h"

class Server {

private:

    WSADATA winSockData;
    const int VERSION = 2;
    const int MODIFICATION = 2;
    WORD winsSockVersion = MAKEWORD(VERSION, MODIFICATION);
    const int PORT = 5223;
    SOCKET listeningSocket;

    std::mutex mutex;

    std::vector <Connection*> connections;
    std::vector <std::string> buffer;

    Server();
    ~Server();

    void adjustWsaData();
    void adjustListeningSocket();
    void bindSocketHint();

public:

    Server(const Server&) = delete;
    Server operator=(const Server&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

    static Server& getInstance();

    void start();

    void addMessage(std::string message);

    void deleteConnection(Connection *connection);

    void shutDownAllConnections();

};

#endif //SERVER_SERVER_H

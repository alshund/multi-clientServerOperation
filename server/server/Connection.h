//
// Created by shund on 15.10.2017.
//

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <ws2tcpip.h>

#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>

class Connection {
private:

    SOCKET clientSocket;
    char *IP;

    bool isActive = true;

    void addMessage(std::string message);
    void threadTimer(std::string threadId);

    std::string idToString();

public:

    Connection(SOCKET &clientSocket, char *IP);

    virtual ~Connection();

    void clientProcessing();
    void setIsActive(bool isActive);
    void closeSocket();

    std::string getPreamble();
    std::string getCurrentTime();
};


#endif //SERVER_CONNECTION_H

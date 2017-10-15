//
// Created by shund on 15.10.2017.
//

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <mutex>
#include <chrono>

#include "Server.h"

class Connection {
private:

    SOCKET clientSocket;

    unsigned long clientAddress;

    std::thread::id  threadId;

    std::vector <std::string> &buffer;

    bool isActive = true;

    std::mutex bufferMutex;

    void addMessage(std::string message);

    void threadTimer(std::string threadId);

    std::string addressToString();

    std::string idToString();

public:

    Connection(SOCKET clientSocket, unsigned long clientAddress, std::vector<std::string> &buffer);

    virtual ~Connection();

    void clientProcessing();

};


#endif //SERVER_CONNECTION_H

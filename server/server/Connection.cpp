//
// Created by shund on 15.10.2017.
//

#include <sstream>

#include "Connection.h"
#include "Server.h"

Connection::Connection(SOCKET clientSocket, unsigned long clientAddress) : clientSocket(clientSocket),
                                                                           clientAddress(clientAddress) {
}

Connection::~Connection() {

    auto &server = Server::getInstance();
    server.deleteConnection(this);
}

void Connection::addMessage(std::string message) {

    auto &server = Server::getInstance();
    server.addMessage(message);
}

void Connection::threadTimer(std::string threadId) {

    std::string stringThreadId = idToString();
    while (isActive) {
        std::string timerData = "[" +  threadId + "]: idle\n";
        std::cout << timerData;
        addMessage(timerData);
        Sleep(1000);
    }
}

std::string Connection::addressToString() {

    std::stringstream stringStream;
    stringStream << clientAddress;
    return stringStream.str();
}

std::string Connection::idToString() {
    std::stringstream stringStream;
    stringStream << std::this_thread::get_id();
    return stringStream.str();
}

void Connection::clientProcessing() {

    std::string connectionMessage = "[" + idToString() + "]: accept new client " + addressToString() + "\n";
    std::cout << connectionMessage;
    addMessage(connectionMessage);

    std::thread timerThread (&Connection::threadTimer, this, idToString());
    timerThread.detach();

    while (isActive) {
        char clientMessage[100];
        int bytesReceived = recv(clientSocket, clientMessage, 100, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Data could't received, Err #" << WSAGetLastError() << std::endl;
        }

        if (bytesReceived == 0) {
            setIsActive(false);
            connectionMessage = "[" + idToString() + "]: client " + addressToString() + " disconnected\n";
            std::cout << connectionMessage;
            addMessage(connectionMessage);
        } else {
            std::cout << "[" + idToString() + "]: " << std::string(clientMessage, 0, bytesReceived) << std::endl;
            addMessage(std::string(clientMessage, 0, bytesReceived));
            send(clientSocket, clientMessage, bytesReceived + 1, 0);
        }
    }
    delete this;
}

void Connection::setIsActive(bool isActive) {
    
    Connection::isActive = isActive;
}

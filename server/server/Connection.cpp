//
// Created by shund on 15.10.2017.
//

#include <sstream>
#include <inaddr.h>

#include "Connection.h"
#include "Server.h"

Connection::Connection(SOCKET &clientSocket, char *IP) : clientSocket(clientSocket), IP(IP) {}

Connection::~Connection() {

    auto &server = Server::getInstance();
    server.deleteConnection(this);
}

void Connection::addMessage(std::string message) {

    auto &server = Server::getInstance();
    server.addMessage(message);
}

void Connection::threadTimer(std::string threadId) {

    while (isActive) {
        std::string timerData = getCurrentTime() + "[" + threadId + "]: " + IP + " idle\n";
        std::cout << timerData;
        addMessage(timerData);
        Sleep(1000);
    }
}

std::string Connection::idToString() {
    std::stringstream stringStream;
    stringStream << std::this_thread::get_id();
    return stringStream.str();
}

void Connection::clientProcessing() {

    std::string connectionMessage = getCurrentTime() + "[" + idToString() + "]: accept new client " + IP + "\n";
    std::cout << connectionMessage;
    addMessage(connectionMessage);

    std::thread timerThread(&Connection::threadTimer, this, idToString());
    timerThread.detach();

    while (isActive) {
        char clientMessage[10000];
        int bytesReceived = recv(clientSocket, clientMessage, 1000, 0);
        if (bytesReceived == SOCKET_ERROR) {
            setIsActive(false);
        } else if (bytesReceived == 0) {
            setIsActive(false);
            connectionMessage = getCurrentTime() + "[" + idToString() + "]: client " + IP + " disconnected\n";
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

void Connection::closeSocket() {
    closesocket(clientSocket);
}

std::string Connection::getCurrentTime() {
    SYSTEMTIME system_time;
    std::stringstream stream;
    GetSystemTime(&system_time);
    stream << "[" << system_time.wHour << ":" << system_time.wMinute << ":" << system_time.wSecond << "]";
    return stream.str();
}

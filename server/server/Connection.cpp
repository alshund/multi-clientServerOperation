//
// Created by shund on 15.10.2017.
//

#include <sstream>

#include "Connection.h"

Connection::Connection(SOCKET clientSocket, unsigned long clientAddress, std::vector<std::string> &buffer)
        : clientSocket(clientSocket), clientAddress(clientAddress), buffer(buffer) {}

Connection::~Connection() {

}

void Connection::addMessage(std::string message) {
    bufferMutex.lock();
    buffer.push_back(message);
    bufferMutex.unlock();
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

    while (true) {
        char clientMessage[100];
        int bytesReceived = recv(clientSocket, clientMessage, 100, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Data could't received, Err #" << WSAGetLastError() << std::endl;
        }

        if (bytesReceived == 0) {
            isActive = false;
            TerminateThread(reinterpret_cast<HANDLE>(timerThread.native_handle()), 0);
            connectionMessage = "[" + idToString() + "]: client " + addressToString() + " disconnected\n";
            std::cout << connectionMessage << std::endl;
            addMessage(connectionMessage);

            break;
        } else{
            std::cout << "[" + idToString() + "]: " << std::string(clientMessage, 0, bytesReceived) << std::endl;
            addMessage(std::string(clientMessage, 0, bytesReceived));
            send(clientSocket, clientMessage, bytesReceived + 1, 0);
        }

    }
    delete this;
}

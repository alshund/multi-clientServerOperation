#include <iostream>
#include <ws2tcpip.h>
#include <inaddr.h>

#pragma comment(lib, "ws_32.lib")

using namespace std;

int main() {

    //Initialize winsock
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);

    int wsaStartup = WSAStartup(version, &wsData);
    if (wsaStartup != 0) {
        cerr << "Can't initialize winsock!" << endl;
        return -1;
    }

    //Create a socket
    SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket == INVALID_SOCKET) {
        cerr << "Can't create a socket" << endl;
        return -1;
    }

    //Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(5223);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listeningSocket, (sockaddr*) &hint, sizeof(hint));

    //Tell the socket to listen for incoming connections
    listen(listeningSocket, SOMAXCONN);

    //Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET  clientSocket = accept(listeningSocket, (sockaddr*) &client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Client could't connect" << endl;
        return -1;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    }
    else {
        //TODO
    }

    //Close listening socket
    closesocket(listeningSocket);

    //While loop: accept and echo message back to client
    char buffer[5000];

    while(true) {
        ZeroMemory(buffer, 5000);

        //Wait for client to send data
        int bytesReceived = recv(clientSocket, buffer, 5000, 0);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Error in recv()" << endl;
            break;
        }

        if (bytesReceived == 0) {
            cout << "Client disconnected" << endl;
            break;
        }

        //Echo message back to client
        send(clientSocket, buffer, bytesReceived + 1, 0);


    }

    //Close the socket
    closesocket(listeningSocket);

    //Cleanup winsock
    WSACleanup();

    return 0;
}
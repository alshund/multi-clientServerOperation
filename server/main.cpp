#include <iostream>
#include <ws2tcpip.h>
#include <thread>
#include <inaddr.h>
#include <vector>
#include <hash_map>
#include <map>

#pragma comment(lib, "ws_32.lib")

using namespace std;


class Connection {

    SOCKET clientSocket;
    u_long address;
public:

    Connection(SOCKET clientSocket, u_long address) : clientSocket(clientSocket), address(address) {}

public:
    void clientProcessing() {

        while (true) {
            string stringBuffer;

            char buffer[4096];
            // string buffer;
            int bytesReceived = recv(clientSocket, buffer, 4096, 0);

            if (bytesReceived == SOCKET_ERROR) {
                cerr << "Error in recv()" << endl;
            }
//
            if (bytesReceived == 0) {
                cout << "[" << this_thread::get_id() << "]: client " << address << " disconnected" << endl;
                break;
            }
            cout << "Client> " << string(buffer, 0, bytesReceived) << endl;
            //Echo message back to client
            send(clientSocket, buffer, bytesReceived + 1, 0);
        }
//    while(true) {
//        Sleep(1000);
//        cout << "1" << endl;
//    }
    }
};

class Server {
private:
    vector<string> buffer ;
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);
    SOCKET listeningSocket;

    void adjust() {
        int wsaStartup = WSAStartup(version, &wsData);
        if (wsaStartup != 0) {
            cerr << "Can't initialize winsock!" << endl;
        }

        //Create a socket
        listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (listeningSocket == INVALID_SOCKET) {
            cerr << "Can't create a socket" << endl;
        }

        //Bind the ip address and port to a socket
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(5223);
        hint.sin_addr.S_un.S_addr = INADDR_ANY;

        bind(listeningSocket, (sockaddr *) &hint, sizeof(hint));

        //Tell the socket to listen for incoming connections
        listen(listeningSocket, SOMAXCONN);
    }

public:
    void start() {
        adjust();
        while (true) {
            //Wat for a connection
            sockaddr_in clientHint;
            int clientHintSize = sizeof(clientHint);
            SOCKET clientSocket = accept(listeningSocket, (sockaddr *) &clientHint, &clientHintSize);
            if (clientSocket == INVALID_SOCKET) {
                cerr << "Client could'n connect, Err #" << WSAGetLastError() << endl;
            }

//        pthread_t thread;
//        pthread_create(&thread, NULL, clientProcessing, &thread);
            Connection *connection = new Connection(clientSocket, clientHint.sin_addr.S_un.S_addr);
            thread t1(&Connection::clientProcessing,ref(connection));
            t1.join();
        }
        WSACleanup();
    }
};

void kek(vector< map <u_long, string> > buffer) {
    string line = "mamapapaprivet";
    buffer[]
}

int main() {
//    Server *server = new Server();
//    server->start();
    vector < map < u_long, string > > buffer;

    return 0;
}



//    //Wait for a connection
//    sockaddr_in client;
//    int clientSize = sizeof(client);
//
//    SOCKET  clientSocket = accept(listeningSocket, (sockaddr*) &client, &clientSize);
//    if (clientSocket == INVALID_SOCKET) {
//        cerr << "Client could't connect" << endl;
//        return -1;
//    }
//
//    char host[NI_MAXHOST];
//    char service[NI_MAXSERV];
//
//    ZeroMemory(host, NI_MAXHOST);
//    ZeroMemory(service, NI_MAXSERV);
//
//    if (getnameinfo((sockaddr*) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
//        cout << host << " connected on port " << service << endl;
//    }
//    else {
//    }
//
//    //Close listening socket
//    closesocket(listeningSocket);
//
//    //While loop: accept and echo message back to client
//    char buffer[5000];
//
//    while(true) {
//        ZeroMemory(buffer, 5000);
//
//        //Wait for client to send data
//        int bytesReceived = recv(clientSocket, buffer, 5000, 0);
//        if (bytesReceived == SOCKET_ERROR) {
//            cerr << "Error in recv()" << endl;
//            break;
//        }
//
//        if (bytesReceived == 0) {
//            cout << "Client disconnected" << endl;
//            break;
//        }
//
//        //Echo message back to client
//        send(clientSocket, buffer, bytesReceived + 1, 0);
//
//
//    }
//
//    //Close the socket
//    closesocket(listeningSocket);
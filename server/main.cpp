#include <iostream>
#include <ws2tcpip.h>
#include <thread>
#include <functional>
#include <sstream>
#include <vector>
#include <mutex>

#pragma comment(lib, "ws_32.lib")

using namespace std;

BOOL WINAPI ConsoleHandler(DWORD);

class Connection {
private:
    SOCKET clientSocket;
    unsigned long address;
    vector <string> &buffer;
    mutex bufferMutex;
public:

    Connection(SOCKET clientSocket, u_long address, vector<string> &buffer) : clientSocket(clientSocket),
                                                                              address(address), buffer(buffer) {}
public:
    void clientProcessing() {
        string tmp = "[" + getStringId() + "]: accept new client " + getStringAddress() + "\n";
        cout << tmp;
        addMessage(tmp);
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
                tmp =  "[" + getStringId() + "]: client " + getStringAddress() + " disconnected\n";
                cout<<tmp;
                addMessage(tmp);
                break;
            }
            cout << "[" + getStringId() + "]:" << string(buffer, 0, bytesReceived) << endl;
            //Echo message back to client
            send(clientSocket, buffer, bytesReceived + 1, 0);
        }

//    while(true) {
//        Sleep(1000);
//        cout << "1" << endl;
//    }
    }
private:
    void addMessage(string message) {
            bufferMutex.lock();
            buffer.push_back(message);
            bufferMutex.unlock();
    }

    string getStringAddress() {
        stringstream stringStream;
        stringStream << address;
        return stringStream.str();
    }

    string getStringId() {
        stringstream stringStream;
        stringStream << this_thread::get_id();
        return stringStream.str();
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

            Connection *connection = new Connection(clientSocket, clientHint.sin_addr.S_un.S_addr, buffer);
            thread t1(&Connection::clientProcessing,ref(connection));
            t1.join();
        }
        WSACleanup();
    }

};

Server *server = new Server();
void my_handler (int param)
{
    delete server;
    system("Pause");
    exit(0);
}
int main() {
    signal(SIGINT, my_handler);
    server->start();
    return 0;
}

#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT  0x501

#include <windows.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Connect{
    protected:
        WSADATA wsaData;
        SOCKET  Csocket;
        std::string port;
        std::string host;
        int result;
        unsigned int type;
        unsigned int family;
        unsigned int protocol;
        bool flag;
    public:
        Connect();
        Connect(std::string h, std::string p);
        ~Connect();
        void setPort(std::string p);
        void setHost(std::string h);
        void setTCP();
        void setUDP();
        std::string getPort();
        std::string getHost();
};

class Server : public Connect{
    public:
        using Connect::Connect;
};

class Client : public Connect{
    public:
        using Connect::Connect;
        void connectToServer();
        void sendMSG(std::string msg);
        std::string response();
};

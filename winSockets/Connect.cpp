#include "Connect.h"

Connect::Connect(){
    host="";
    port="";
}

Connect::Connect(std::string h, std::string p){
    host = h;
    port = p;
}

Connect::~Connect(){

}

void Connect::setPort(std::string p){
    port = p;
}

void Connect::setHost(std::string h){
    host = h;
}

void Connect::setTCP(){
    protocol = IPPROTO_TCP;
    type = SOCK_STREAM;
    family = AF_UNSPEC;
}

void Connect::setUDP(){

}

std::string Connect::getPort(){
    return port;
}

std::string Connect::getHost(){
    return host;
}

void Client::connectToServer(){
    struct addrinfo *addrs = NULL, *ptr = NULL, hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = family;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result) {
        printf("WSAStartup failed with error: %d\n", result);
        flag=false;
        return;
    }    

    result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addrs);
    if (result) {
        WSACleanup();
        flag=false;
        return;
    }

    for(ptr=addrs; ptr != NULL ;ptr=ptr->ai_next) {
        Csocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Csocket == INVALID_SOCKET) {
            WSACleanup();
            flag=false;
            return;
        }

        result = connect(Csocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (result == SOCKET_ERROR) {
            closesocket(Csocket);
            Csocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(addrs);

    if (Csocket == INVALID_SOCKET) {
        WSACleanup();
        flag=false;
        return;
    }

    flag=true;
}

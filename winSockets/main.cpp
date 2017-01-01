#include "Connect.h"
#include <iostream>

using namespace std;

int main(){

    Client google("www.google.com", "80");
    google.setTCP();
    google.connectToServer();
    //google.sendMSG("HELLO");
    //cout<<google.response()<<endl;

    Sleep(5000);

    return 0;
}

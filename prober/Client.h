#pragma once

//includes
#include "Connect.h"

//uses chunk end check only if specified upon compilation

//class client extends the connect class
class Client : public Connect {
public:
	Client(std::string h, std::string p);
	void estConnection();       //connect to the server
    #ifdef CHUNK bool chunkEnd(); #endif    //if set checks for end of response 
    std::string response();     //get response from server
};

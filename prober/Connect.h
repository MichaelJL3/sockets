#pragma once

#ifdef ERR
#include <cstdio>
#include <errno.h>
#endif

//includes
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

//connection class
class Connect {
protected:  //variables
	struct timeval timeout;     //timeout struct
	std::string port;           //port
	std::string host;           //host
	int Csocket;                //socket
	int result;                 //result of operation
	unsigned int bufferSize;    //size of character buffer
	unsigned int type;          //IP type
	unsigned int family;        //IP family
	unsigned int protocol;      //IP protocol
	char *recvBuff;             //buffer for recieving messages
public:     //functions
	Connect();                              //default constructor
	Connect(std::string h, std::string p);  //overload with host/port
	~Connect();                             //destructor
	bool isOpen();                          //check if connection is open
	void finishedSending();                 //shutdown write to socket
	void sendMSG(std::string msg);          //send message to server
	void setBufferSize(unsigned int n);     //set the buffer size
	void setTimeoutSec(int tm);             //set the timeout in seconds
	void setTimeoutUSec(int tm);            //set the timeout in nano seconds
	void setPort(std::string p);            //set the port
	void setHost(std::string h);            //set the host
	void setTCP();                          //set to TCP
	void setUDP();                          //set to UDP
	void closeConnection();                 //close the connection
	void allocateBuff();                    //allocate to the buffer
	void deleteBuff();                      //delete the buffer
	int getResult();                        //get the result of last operation
	int getTimeoutSec();                    //get the timeout in seconds
    int getTimeoutUSec();                   //get the timeout in nano seconds
	std::string getPort();                  //get the set port
	std::string getHost();                  //get the set host
};

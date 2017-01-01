#include "Connect.h"

//default constructor for connection object
Connect::Connect() {
	host = "";
	port = "";
	timeout.tv_sec=2;
	timeout.tv_usec=0;
	bufferSize = 1024;
	allocateBuff();
	setTCP();
}

//overloaded constructor using set host/port
Connect::Connect(std::string h, std::string p) {
	host = h;
	port = p;
    //set the default timeout
	timeout.tv_sec=2;
	timeout.tv_usec=0;
    //set the default buffer size
	bufferSize = 1024;
    //allocate the buffer
	allocateBuff();
    //set TCP protocol default
	setTCP();
}

//destructor
Connect::~Connect() {
    //delete the allocated buffer
	deleteBuff();
    //close the connection
	closeConnection();
}

//allocate the char array
void Connect::allocateBuff(){
	recvBuff = new char[bufferSize];
}

//delete the buffer
void Connect::deleteBuff(){
	delete recvBuff;
}

//close the socket
void Connect::closeConnection() {
	if (isOpen()) 
		close(Csocket);
}

//set the buffersize
void Connect::setBufferSize(unsigned int n) {
	bufferSize = n;
}

//set the port
void Connect::setPort(std::string p) {
	port = p;
}

//set the host
void Connect::setHost(std::string h) {
	host = h;
}

//TCP flags
void Connect::setTCP() {
	protocol = IPPROTO_TCP;
	type = SOCK_STREAM;
	family = AF_UNSPEC;
}

//UDP flags
void Connect::setUDP() {
	protocol = IPPROTO_UDP;
	type = SOCK_DGRAM;
	family = AF_UNSPEC;
}

//set the timeouts seconds
void Connect::setTimeoutSec(int tm) {
	timeout.tv_sec = tm;
}

//set the timeouts nano seconds
void Connect::setTimeoutUSec(int tm){
	timeout.tv_usec = tm;
}

//send a message to the server
void Connect::sendMSG(std::string msg) {
    //if the connection is open send the message
	if (isOpen()) {
		result = send(Csocket, msg.c_str(), msg.length(), 0);
	}
}

//option for closing the write dialog
void Connect::finishedSending() {
    //if it fails to close shutdown the connection
	result = shutdown(Csocket, SHUT_WR);
	if (result < 0) {
		#ifdef ERR  //if error debug is set then display unix error
		fprintf(stderr, "[Error] > %s\n", strerror(errno));
		#endif
		closeConnection();
	}
}

//check if connection is open
bool Connect::isOpen() {
	if (Csocket < 0)
		return false;
	return true;
}

//get result of last operation
int Connect::getResult() {
	return result;
}

//get the set timeout in seconds
int Connect::getTimeoutSec(){
	return timeout.tv_sec;
}

//get the set timeout in nano seconds
int Connect::getTimeoutUSec(){
	return timeout.tv_usec;
}

//get the set port
std::string Connect::getPort() {
	return port;
}

//get the set host
std::string Connect::getHost() {
	return host;
}

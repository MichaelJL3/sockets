#include "Client.h"

//Constructor takes a string host/port to setup basic connections
Client::Client(std::string h, std::string p){
	//set default timeout in seconds 
	//set default char buffer size
	//allocate char buffer
	
	host = h;
	port = p;
	timeout.tv_sec=2;
	timeout.tv_usec=0;
	bufferSize = 1024;
	allocateBuff();
	setTCP();
}

//Attempt to connect to the server 
void Client::estConnection() {
	struct addrinfo *addrs = NULL, *ptr = NULL, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = type;
	hints.ai_protocol = protocol;

	//get the address list from dns of given host name
	result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addrs);
	if (result){
		#ifdef ERR 	//if error debug is set then display unix error
		fprintf(stderr, "[Error] > %s\n", strerror(errno));
		#endif
		return;
	}

	//look for valid address in list
	for (ptr = addrs; ptr != NULL; ptr = ptr->ai_next) {
		Csocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (Csocket < 0) {
			#ifdef ERR 	//if error debug is set then display unix error
			fprintf(stderr, "[Error] > %s\n", strerror(errno));
			#endif
			return;
		}

		result = connect(Csocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result < 0) {
			close(Csocket);
			Csocket = -1;
			continue;
		}
		break;
	}

	//free the allocated address space
	freeaddrinfo(addrs);

	if (Csocket < 0){
		#ifdef ERR  //if error debug is set then display unix error
		fprintf(stderr, "[Error] > %s\n", strerror(errno));
		#endif
		return;
	}

	//set the socket recv 
	result = setsockopt(Csocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	if(result < 0) {
		#ifdef ERR  //if error debug is set then display unix error
		fprintf(stderr, "[Error] > %s\n", strerror(errno));
		#endif
		return;
	}
}

#ifdef CHUNK
//If encoding is chunked check for delimiting \r\n\r\n saves a 2 sec timeout
bool Client::chunkEnd(){
	//can only judge a set of data which has at least a minimum of four char
	if(result<4)
		return false;

	//check for \r\n\r\n delimiter
	if(recvBuff[result-1]=='\n'&&recvBuff[result-2]=='\r')
		if(recvBuff[result-3]=='\n'&&recvBuff[result-4])
			return true;

	return false;
}
#endif

std::string Client::response() {
	std::stringstream recvStream;

	//if the connection is open loop and retrieve data from the server
	if (isOpen()) {
		do {
			result = recv(Csocket, recvBuff, bufferSize, 0);
			//if theres an error exit the loop
			if (result <= 0){ 
				#ifdef ERR  //if error debug is set then display unix error
				fprintf(stderr, "[Error] > %s\n", strerror(errno));
				#endif
				result = 0;
			}
			//else save the results and check for end of chunked encode
			else if (result){
				recvStream.write(recvBuff, result);
				#ifdef CHUNK
				if(chunkEnd())
					break;
				#endif
			}
			
		} while (result);
	} 

	return recvStream.str();
}

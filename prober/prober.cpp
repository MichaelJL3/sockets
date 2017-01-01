#include <cstdio>
#include <fstream>
#include <ctime>
#include "Client.h"

#include <iostream>

//compile with -D Debug 
#ifdef DEBUG
#define ERR
#endif

#ifdef MONITOR
#define ACTIVE
#endif

#ifdef ENCODE
#define CHUNK
#endif

#define REQTYPE "GET"
#define PROTOCOL "HTTP/1.1"
#define PORT "80"
#define WAIT 30
#define EXT ".txt"

//get the return status of a server response
std::string getStatus(std::string res){
	int size=res.length();
	int status=0;
	int i=0;
	std::string code="";

	//if no size return -1
	if(!size)
		return "-1";

	//get the location of the first space
	status=res.find(" ");
	if(status==std::string::npos)
		return "-1";

	//move the string to that location
	res=res.substr(status+1);

	//grab all the digits of the status code
	while(isdigit(res[i]))
		code+=res[i++];

	return code;
}

//main process
int main(int argv, char** argc){
	unsigned int allowed=0;		//max iterations based on timeout and wait
	unsigned int holder=0;		//holder to keep track of iterations
	struct timespec slp, slpbuf;	//used to set the wait between requests
	std::string path="/";		//default path to build off
	time_t tm;			//time struct to get current time

	//if not enough arguments end program
	if(argv<2){
		puts("Incorrect Usage ex: ./probber http://www.google.com /index.html");
		return 1;
	}
	//if a path is provided parse it
	if(argv>2){
		if(argc[2][0]=='/')
			path=argc[2];
		else
			path+=argc[2];
	}

	//set the wait time
	slp.tv_sec=WAIT;
	slp.tv_nsec=0;

	//create and connect client
	Client connection(argc[1], PORT);
	connection.estConnection();
	
	//max number of iterations to allow
	allowed=WAIT/connection.getTimeoutSec()-1;

	//request string
	std::string res="";
	std::string req=REQTYPE;
	req+=" "+path+" "+std::string(PROTOCOL);
	req+="\r\n\r\n";

	//create the log file
	std::ofstream log;
	std::string fileName=path.substr(1);
	fileName+=EXT;

	//if the log doesn't open exit program
	log.open(fileName.c_str());
	if(!log.is_open()){
		puts("Error opening log file exiting application");
		return 1;
	}
	log<<"URL="<<argc[1]<<std::endl;
	
	#ifdef ACTIVE 
	std::cout<<"Starting [Attempts]: "<<allowed<<"\n";
	#endif

	//never ending loop
	while(true){
		//get the current time
		tm=time(0);

		//open connection if closed
		if(!connection.isOpen())
			connection.estConnection();

		//send the request
		connection.sendMSG(req);
		holder=allowed;
		res="";

		//add if statement with send success
		do{
			res=connection.response();
		}while(connection.isOpen()&&!res.length()&&holder--);

		log<<tm<<","<<getStatus(res)<<std::endl;
		
		#ifdef ACTIVE
		std::cout<<tm<<","<<getStatus(res);
		std::cout<<" [State]: "<<connection.isOpen()<<"\n";
		#endif 

		//determine how much time is still left to wait
		slp.tv_sec=WAIT-(time(0)-tm);

		//if theres time then wait
		if(slp.tv_sec>0)
			nanosleep(&slp, &slpbuf);
	}

	//CANNOT OCCUR
	log.close();
	connection.closeConnection();

	return 0;
}

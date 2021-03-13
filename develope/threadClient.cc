// hearder
#include "socket.h"
#include <iostream>
#include <thread>

void recvMesg(ClientSocket& cs)
{
	std::string buff;
	while( cs << buff )
	{
		//sleep(1);
		std::cout << buff;
	}
	std::cout << "server close" << std::endl ;
	exit(0);
}

int main(int argc ,char **argv){
	ClientSocket cs;	
	std::cout << "connec success";
	std::string str;

	//std::thread reaceiver{ recvMesg, std::ref(cs)};

	while( std::getline(std::cin,str) ) {
		if( str == "exit" ) break;
		cs << str << flush;
	}
	std::cout << "client close" << std::endl;
	exit(0);

	//reaceiver.join();
}


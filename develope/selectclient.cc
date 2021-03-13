// hearder
#include "socket.h"
#include <iostream>

int main(int argc ,char **argv){
	ClientSocket cs;	
	std::cout << "connec success" << std::endl;
	std::string str;

	// fd set configure
	fd_set mfds ,rfds;
	FD_ZERO(&mfds);
	FD_ZERO(&rfds);
	FD_SET(cs, &mfds);
	FD_SET( 0, &mfds);

	int maxfd = cs + 1 , ret;
	rfds = mfds;
	
	while( select( ret = maxfd, &rfds, NULL, NULL, NULL) )
	{
		if(FD_ISSET(0,&rfds))
		{
			std::getline(std::cin,str);
			if( str == "exit" ) {
				std::cout << "client close" << std::endl;
				exit(0);
			}
			else  
				cs << str << flush;
		}
		if(FD_ISSET(cs, &rfds) )
		{
			if( cs >> str)
				std::cout << str << std::endl;
			else{
				std::cout << "server close";
				exit(1);
			}
		}
		rfds = mfds;
	}

}


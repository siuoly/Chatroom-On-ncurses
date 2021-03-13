// hearder
#include "socket.h"
#include <iostream>
#include <list>

struct SockName{
	AcceptSocket as;
	std::string name;
};

void multcast(const std::string& buf, 
		std::list<AcceptSocket>& fdlist)
{
	std::cout << "multi cast";
	for(auto& sock:fdlist)
		sock << buf <<  flush;
}

void server(){
	ListenSocket ls;
	
	fd_set mfds ,rfds;

	FD_ZERO(&mfds);
	FD_ZERO(&rfds);

	FD_SET(ls, &mfds);
	FD_SET( 0, &mfds);

	int maxfd = ls + 1 , ret;
	rfds = mfds;

	std::list<AcceptSocket> fdlist;

	while( ret = select( maxfd, &rfds, NULL , NULL, NULL))
	{
		if(ret == -1){ perror("select"); exit(1); }

		if( FD_ISSET( 0 , &rfds ) ){
			std::string s; 
			std::cin >> s;
			if( s == "exit")
			break;
		}

		// listen socket 
		if( FD_ISSET( ls , &rfds ) ){
			std::cout << "accept new sock " ;
			fdlist.push_back( AcceptSocket(ls) );
			std::cout << static_cast<int>(fdlist.back()) << std::endl;
			maxfd = fdlist.back() + 1 ;
			FD_SET( fdlist.back(), &mfds);
		}

		// read from other socket
		for(auto sock = fdlist.begin(); sock!= fdlist.end(); ++sock )
		{
			if( FD_ISSET( *sock, &rfds) )
			{
				std::string buf;
				if( *sock >> buf )
				{
					std::cout << buf <<std::endl ;
					multcast(buf, fdlist);	
				}
			}
		}
		
		//check for unused socket
		for(auto it = fdlist.begin(); it!= fdlist.end(); ++it )
		{
			if( !it->connecting )
			{ 
				FD_CLR(*it, &mfds);
				std::cout << "close " <<static_cast<int>(*it) << std::endl ;
				fdlist.erase(it); 
				break;
			}
		}

		//change maxfd
		maxfd= ls+1 ;
		for(auto& it:fdlist) maxfd = std::max((int)it+1, maxfd);

		rfds = mfds;
	}
	
}

int main(int argc ,char **argv){
	server();
}




// create socket

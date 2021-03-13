// hearder
#include "socket.h"
#include <iostream>
#include <list>

//send message to eveyone
void multcast(const std::string& message, 
		std::list<AcceptSocket>& fdlist)
{
	for(auto& sock:fdlist)
		sock << message << flush;
	std::cout << "multi cast {" << message <<"}\n";
}

//send users message to client
void UsrsInform(std::list<AcceptSocket>& fdlist){
	std::string message="#usrs#";

	// sort out name message
	for(auto& s: fdlist)
		message += s.name +'\n';

	multcast(message,fdlist);
}

void newUsrInform( std::list<AcceptSocket>& fdlist){
	// get new user name 
	auto &sock = fdlist.back();
	sock.recvname();

	// send message to every one
	std::cout << "send new usr <" << sock.name <<"> enter message\n";

	UsrsInform(fdlist);
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

	while( (ret = select( maxfd, &rfds, NULL , NULL, NULL)) )
	{
		if(ret == -1){ perror("select"); return ; }

		// control server
		if( FD_ISSET( 0 , &rfds ) ){
			std::string s; 
			std::cin >> s;
			if( s == "q")
			break;
		}

		// listen socket 
		if( FD_ISSET( ls , &rfds ) ){
			std::cout << "accept new sock" ;
			fdlist.push_back( AcceptSocket(ls) );
			std::cout << static_cast<int>(fdlist.back()) << std::endl;
			maxfd = fdlist.back() + 1 ;
			FD_SET( fdlist.back(), &mfds);

			newUsrInform(fdlist);
		}

		// read from other socket
		for(auto sock = fdlist.begin(); sock!= fdlist.end(); ++sock )
		{
			if( FD_ISSET( *sock, &rfds) )
			{
				std::string buf;
				if( *sock >> buf )
				{
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

				std::string message = it->name;
				message+=" leave the chat room....";
				multcast(message, fdlist);
				UsrsInform(fdlist);
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

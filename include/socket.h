
#ifndef SOCKET_H
#define SOCKET_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>

struct __not_call_constructor{};

class Socket{
	public:
		int sock;
		bool connecting = true;
		std::string wbuffer;
		char rbuffer[100];

		Socket( __not_call_constructor ){}
		Socket();
		Socket(Socket&& o);

		~Socket(){ if(sock)  close(sock); }

		/* ex:
		 * socket << string << "Hello" << 2+5 <<
		 * 				 << '\n' << flush ; */
		Socket& operator<<(const std::string& mesg){ wbuffer +=  mesg ; return *this; }
		Socket& operator<<( char ch){ wbuffer += ch; return *this; }
		Socket& operator<<( int num ){ wbuffer += std::to_string(num); return *this; }
		Socket& operator<<( void (*f)( Socket& ) ){ f(*this); 
		return *this; }

		/* ex:
		 * socket >> buff; 
		 * std::cout << buff;*/
		Socket& operator>>( std::string& buff);

		operator int(){ return sock;}
		operator bool(){ return connecting; }
		bool operator==(const Socket& o)const{
			return  sock == o.sock;
		}
};

void flush(Socket& sock);

class ListenSocket: public Socket{
	public:
		ListenSocket(int port = 5566, int qlen = 5 );
	private:
		struct sockaddr_in addr;
};

class AcceptSocket: public Socket{
	public:
		std::string name; // client name

		AcceptSocket(ListenSocket& lsock );

		void recvname()
		{ (*this) >> name; }
	private:
		struct sockaddr_in acaddr;
	 	socklen_t alen ;
};

class ClientSocket:public Socket{
	public:
		ClientSocket
			(const char* ip="127.0.0.1", int port = 5566 );
	private:
		struct sockaddr_in addr;
};

#endif 

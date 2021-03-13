
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
		Socket(){
			sock = socket(AF_INET , SOCK_STREAM, 0);
			if( sock <0 ){ perror("socket()"); exit(1); }
		}
		Socket(Socket&& o)
		:sock(o.sock), wbuffer(std::move(o.wbuffer)) 
		{ o.sock = 0; }

		~Socket(){ 
			if(sock)  close(sock);
		}

		/* ex:
		 * socket << string << "Hello" << 2+5 <<
		 * 				 << '\n' << flush ; 				
		 */
		Socket& operator<<(const std::string& mesg){ wbuffer +=  mesg ; return *this; }
		Socket& operator<<( char ch){ wbuffer += ch; return *this; }
		Socket& operator<<( int num ){ wbuffer += std::to_string(num); return *this; }
		Socket& operator<<( void (*f)( Socket& ) ){ f(*this); 
		return *this; }

		/* string buff;
		 * socket >> buff; 
		 * std::cout << buff;*/
		Socket& operator>>( std::string& buff){ 
			int n = read(sock,rbuffer,100); 
			if(n<=0) connecting=false; 
			else 		 buff.assign(rbuffer,n); 
			return *this; 
		}

		operator int(){ return sock;}
		operator bool(){ return connecting; }
		bool operator==(const Socket& o)const{
			return  sock == o.sock;
		}
};

void flush(Socket& sock){
	auto &buffer = sock.wbuffer;
	if( write(sock ,buffer.c_str(), buffer.size()) !=
			buffer.size() ){
		perror("write"); exit(1);
	}
	buffer.clear();
}

class ListenSocket: public Socket{
	public:
		ListenSocket(int port = 5566, int qlen = 5 ){
			addr.sin_family = AF_INET;
			addr.sin_port = htons( port );
			addr.sin_addr.s_addr = htons( INADDR_ANY );

			int ret = bind( sock, (struct sockaddr*)&addr, sizeof(addr) );
			if( ret <0)
			{ perror("bind()"); exit(1); }

			ret = listen( sock, qlen );
			if( ret != 0) { perror("listen()"); exit(1); }

		}
	private:
		struct sockaddr_in addr;
};

class AcceptSocket: public Socket{
	public:
		AcceptSocket(ListenSocket& lsock )
			:Socket( __not_call_constructor() )
		{
			socklen_t alen = sizeof(acaddr); 
			sock = accept(lsock, (struct sockaddr*)&acaddr, &alen);
			if( sock <0 ){ perror("accept()"); exit(1);} 
		}
	private:
		struct sockaddr_in acaddr;
	 	socklen_t alen ;
};

class ClientSocket:public Socket{
	public:
		ClientSocket
			(const char* ip="127.0.0.1", int port = 5566 ){
				// set address
				bzero( &addr, sizeof(addr));
				addr.sin_family = AF_INET;
				addr.sin_port = htons(port);
				int ret = inet_pton( AF_INET, ip , &addr.sin_addr);
				if( ret != 1 ){
					perror( " set_adderss inet_pton()");
					exit(1);
				}

				//connect	
				ret =connect(sock,(struct sockaddr*)&addr, sizeof(addr));
				if( ret < 0 ){ perror("connect"); exit(1); }
				
			}
	private:
		struct sockaddr_in addr;
};

#endif 

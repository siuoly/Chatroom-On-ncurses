
#include "socket.h"

// socket class
Socket::Socket(){
	sock = socket(AF_INET , SOCK_STREAM, 0);
	if( sock <0 ){ perror("socket()"); exit(1); }
}

Socket::Socket(Socket&& o)
	:sock(o.sock), wbuffer(std::move(o.wbuffer)) 
{ o.sock = 0; }


Socket& Socket::operator>>( std::string& buff){ 
	int n = read(sock,rbuffer,100); 
	if(n<=0) connecting=false; 
	else 		 buff.assign(rbuffer,n); 
	return *this; 
}

// ex: sock << message << flush ;
void flush(Socket& sock){
	auto &buffer = sock.wbuffer;
	if( write(sock ,buffer.c_str(), buffer.size()) !=
			buffer.size() ){
		perror("write"); exit(1);
	}
	buffer.clear();
}

// ListenSocket class
ListenSocket::ListenSocket(int port, int qlen){
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = htons( INADDR_ANY );

	int ret = bind( sock, (struct sockaddr*)&addr, sizeof(addr) );
	if( ret <0)
	{ perror("bind()"); exit(1); }

	ret = listen( sock, qlen );
	if( ret != 0) { perror("listen()"); exit(1); }

}

// AcceptSocket class
AcceptSocket::AcceptSocket(ListenSocket& lsock )
	:Socket( __not_call_constructor() )
{
	socklen_t alen = sizeof(acaddr); 
	sock = accept(lsock, (struct sockaddr*)&acaddr, &alen);
	if( sock <0 ){ perror("accept()"); exit(1);} 
}

// ClientSocket class
ClientSocket::ClientSocket
(const char* ip, int port){
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

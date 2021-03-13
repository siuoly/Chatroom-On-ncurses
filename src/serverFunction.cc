#include "serverFunction.h"

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


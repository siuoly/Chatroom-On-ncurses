
#ifndef SERVERFUCNTION_H
#define SERVERFUCNTION_H
#include "socket.h"
#include <list>
#include <iostream>

void multcast(const std::string& , 
		std::list<AcceptSocket>& );
void UsrsInform(std::list<AcceptSocket>& );
void newUsrInform( std::list<AcceptSocket>& );

#endif 

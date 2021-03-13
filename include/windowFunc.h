
#ifndef WINDOWFUCN_H
#define WINDOWFUCN_H

#include "win.h"
#include <string>

std::string getName(Win& win);
void UsersUpdate(Win& win,const std::string& buff);
void update( WINDOW* w);

#endif 

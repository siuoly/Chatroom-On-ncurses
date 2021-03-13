#include "windowFunc.h"
void UsersUpdate(Win& win,const std::string& buff){
	win.clr();
	win << buff << update;
}

std::string getName(Win& win)
{
	win.clr(); win();
	win << "Input your name:" << update;
	std::string usr;
	win >> usr ;
	// reset screen
	win(0,0); wclrtoeol(win); 
	win << usr << ':'; win();
	return  usr;
}

// usage: win << "123456" << update;
void update( WINDOW* w) { wrefresh(w); }


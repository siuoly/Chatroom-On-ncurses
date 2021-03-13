#include <iostream>
#include <ncurses.h>
#include <string>
#include "win.h"
#include "socket.h"

using namespace std;

void UsersUpdate(Win& win,const string& buff){
	win.clr();
	win << buff << update;
}
std::string getName(Win& win)
{
	win.clr(); win();
	win << "Input your name:" << update;
	string usr;
	win >> usr ;
	// reset screen
	win(0,0); wclrtoeol(win); 
	win << usr << ':'; win();
	return  usr;
}

void client(){

	// initialize screen
	InitSrc screen;

	// initialize layout
	int srcx = screen.col;
	int srcy = screen.row;
	
	int h1=srcy-5, w1=srcx-20;
	int h2=srcy-h1,w2=w1;
	int h3=srcy   ,w3=20;
	Win box1(h1,w1,0,0)  , //left up
			box2(h2,w2,h1,0) , //left down
			box3(h3,w3,0,w1) ; //right 
	box1.addbox(); 
	box2.addbox(); 
	box3.addbox(); 

	Win win1(h1-2,w1-2,1,1),
			win2(h2-2,w2-2,h1+1,1),
			win3(h3-2,w3-2,1,w1+1);

	win2(0,0);
	string usr = getName(win2);	
	// connection
	ClientSocket cs; cs <<usr <<flush;

	// fd set configure
	fd_set mfds ,rfds;
	FD_ZERO(&mfds);
	FD_ZERO(&rfds);
	FD_SET( 0, &mfds);
	FD_SET( cs, &mfds);

	int maxfd = cs+1 , ret;
	rfds = mfds;
	

	string buff;
	while((ret = select(maxfd, &rfds, NULL, NULL, NULL)))
	{
		// client input
		if(FD_ISSET(0,&rfds))
		{
			win2 >> buff;
			if( buff=="clear" )
			{ 
				win1.clr();win2.clr(); 
				win1(); win2(); 
			}
			else if( buff=="") ;
			else if( buff=="q") goto EXIT;
			else cs << usr <<':' << buff << flush;
		}

		// client recv
		if(FD_ISSET(cs,&rfds))
		{
			cs >> buff;
			
			if(!cs)  //error 
				continue; 
			else if(buff.substr(0,6) == "#usrs#") //newUsrInform
				UsersUpdate(win3,buff.substr(6));
			else 			//messae
				win1 << buff << '\n' << update;
		}

		win2(0,0); wclrtoeol(win2); 
		win2 << usr << ':'; win2();

		rfds = mfds;
	}
EXIT:
	;
}

int main( ){
	client();
}

#include <iostream>
#include <ncurses.h>
#include <string>
#include "win.h"
#include "socket.h"

using namespace std;

int main(){

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

	// connection
	ClientSocket cs;	
	cs << "connection" << flush;

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
		if(FD_ISSET(0,&rfds))
		{
			win2 >> buff;
			if( buff=="clear" )
			{ win1.clr();win2.clr(); }
			else if( buff=="") ;
			else if( buff=="q") goto EXIT;
			else cs << buff << '\n' << flush;
			
		}
		if(FD_ISSET(cs,&rfds))
		{
			if(cs >>buff)	
				win1 << buff << '\n' << update;
		}
		win2(0,0); wclrtoeol(win2); win2();

		rfds = mfds;
	}
EXIT:
	;
}

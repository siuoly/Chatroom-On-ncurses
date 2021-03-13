#include <iostream>
#include <ncurses.h>
#include <cstring>

#include "win.h"
using namespace std;

int main(){
	InitSrc screen;
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

	chtype str[30];
	char s[300];
	while(win2 >> s)
	{
		if( strcmp(s , "clear") ==0 )
		{ win1.clr();win2.clr(); }
		else if( strcmp(s,"") == 0) win2(0,0);
		else if( strcmp(s,"q") == 0) break;
		else{
			win2(0,0); wclrtoeol(win2);
			win1 << s << '\n';
		}
	}
}

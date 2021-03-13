
#include "win.h"

Win::~Win(){
	clr();
	wborder (thisWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
#ifdef DEBUG
	mvwprintw(thisWin,0,0,"destor thisWin: height:%d width:%d \
			starty:%d startx:%d", height,width,starty,startx);
	wrefresh(thisWin);
	getch();
#endif 
	delwin(thisWin); 
}


void Win::printf(const char*fmt , ...) {
	va_list args;
	va_start( args, fmt);
	vw_printw(thisWin , fmt, args);
	va_end( args);
}


Win& Win::operator<<( int d ) {
	char s[15]; sprintf(s,"%d",d);
	waddstr( thisWin, s);
	return  *this;
}
Win& Win::operator<<( double d ) {
	char s[15]; sprintf(s,"%f",d);
	waddstr( thisWin, s);
	return  *this;
}
Win& Win::operator<<(char c) {
	waddch(thisWin,c);
	return  *this;
}
Win& Win::operator<<( const char* str) {
	waddstr(thisWin,str); 
	return  *this;
}
Win& Win::operator<<(const std::string& str) {
	waddstr(thisWin, str.c_str());
	return  *this;
}
Win& Win::operator<<(void(*f)( WINDOW* )) {
	f(thisWin);
	return  *this;
}

Win& Win::operator>> (char str[]){
	wgetstr(thisWin, str);
	return *this;
}

Win& Win::operator>> (std::string& str){
	if( wgetnstr(thisWin, rbuffer, 200)==ERR)
	{ perror("wgetnstr"); exit(1); }

	str.assign(rbuffer);
	return *this;
}

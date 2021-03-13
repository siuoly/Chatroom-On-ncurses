#ifndef WIN_H
#define WIN_H
#include <ncurses.h>
#include <string>


class Win{
	public:
		WINDOW* thisWin;
		int height, width, starty,startx ;
		char rbuffer[200];
		

		Win(int h=0,int w=0, int y=0,int x =0 )
			: height(h), width(w), starty(y),startx(x) 
		{
			//if(thisWin)delthisWin(thisWin);
			thisWin = newwin( height, width, starty,startx );
		}

		~Win(){
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

		//usage:  w.printf("3 + 2 = %d, 3+2 );
		void printf(const char*fmt , ...) {
			va_list args;
			va_start( args, fmt);
			vw_printw(thisWin , fmt, args);
			va_end( args);
		}

		//usage: w<< "thisWin size is " << 30 << " and " << 20 ;
		Win& operator<<( int d ) {
			char s[15]; sprintf(s,"%d",d);
			waddstr( thisWin, s);
			return  *this;
		}
		Win& operator<<( double d ) {
			char s[15]; sprintf(s,"%f",d);
			waddstr( thisWin, s);
			return  *this;
		}
		Win& operator<<(char c) {
			waddch(thisWin,c);
			return  *this;
		}
		Win& operator<<( const char* str) {
			waddstr(thisWin,str); 
			return  *this;
		}
		Win& operator<<(const std::string& str) {
			waddstr(thisWin, str.c_str());
			return  *this;
		}
		Win& operator<<(void(*f)( WINDOW* )) {
			f(thisWin);
			return  *this;
		}

		// usage: w >> str;
		Win& operator>> (char str[]){
			wgetstr(thisWin, str);
			return *this;
		}

		Win& operator>> (std::string& str){
			if( wgetnstr(thisWin, rbuffer, 200)==ERR)
			{ perror("wgetnstr"); exit(1); }

			str.assign(rbuffer);
			return *this;
		}

		// usage: while( w ){ ... }
		operator bool(){ return  true; };
		operator WINDOW*(){ return  thisWin;};

		// usage:w.move(20,20);
		void move(int y, int x){ wmove(thisWin,y,x);}
		void operator()(int y, int x){ wmove(thisWin,y,x);}
		
		// refresh
		void refresh(){ wrefresh(thisWin);}
		void operator() (){ wrefresh(thisWin);}

		//box , unbox
		void addbox(){ box(thisWin,0,0); wrefresh(thisWin); }
		void unbox(){ 
			wborder (thisWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		}
		
		//clear
		void clr(){ wclear(thisWin);}

};

// usage: win << "123456" << update;
void update( WINDOW* w) { wrefresh(w); }

class InitSrc{
	public:
		InitSrc():row(0),col(0)
		{
			initscr();
			//cbreak(); // response imediately
			//noecho(); // noecho
			//curs_set(0);
			getmaxyx( stdscr, row, col );
			keypad(stdscr, TRUE); // set keyboard
			refresh();
		}
		~InitSrc(){ endwin(); exit(0); }
		int row,col;
		int y,x;
};
#endif 

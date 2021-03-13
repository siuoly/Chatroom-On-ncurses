
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
			:height(h), width(w), starty(y),startx(x) 
		{
			thisWin = newwin( height, width, starty,startx );
		}

		~Win();

		//usage:  w.printf("3 + 2 = %d, 3+2 );
		void printf(const char*fmt , ...) ;

		//ex: w<< "thisWin size is " << 30 << " and " << 20 << update;
		Win& operator<<( int d ) ;
		Win& operator<<( double d ) ;
		Win& operator<<(char c) ;
		Win& operator<<( const char* str);
		Win& operator<<(const std::string& str) ;
		Win& operator<<(void(*f)( WINDOW* ));

		// usage: w >> str;
		Win& operator>> (char str[]);
		Win& operator>> (std::string& str);

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

class InitSrc{
	public:
		InitSrc():row(0),col(0)
		{
			initscr();
			//cbreak(); // response imediately
			//noecho(); // noecho
			//curs_set(0); // not show mouse
			getmaxyx( stdscr, row, col );
			keypad(stdscr, TRUE); // set keyboard
			refresh();
		}
		~InitSrc(){ endwin(); exit(0); }
		int row,col;
		int y,x;
};
#endif 

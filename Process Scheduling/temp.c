#include <curses.h>

int main()
{
	int maxx, maxy;
	initscr();
	getmaxyx(stdscr, maxy, maxx);
/*
	move(maxy/2, maxx/2);
	printw("Hello World!!!");
*/
	mvprintw(maxy/2, (maxx - 14)/2, "Hello World!!!");
	addch('a');
	getch();
	endwin();
	return 0;
}

#include <stdlib.h>
#include <curses.h>

void rectangle(int y1, int x1, int y2, int x2)
{
	mvhline(y1, x1, 0, x2-x1);
	mvhline(y2, x1, 0, x2-x1);
	mvvline(y1, x1, 0, y2-y1);
	mvvline(y1, x2, 0, y2-y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
}

int main()
{
	int unit, *bt=NULL, i=0, TT, T, n, maxx, maxy, x, y;
	float TAT, WT;
	char file[32];
	FILE *fp;
	initscr();
	getmaxyx(stdscr, maxy, maxx);
	rectangle(0, 0, maxy - 1, maxx - 1);
	mvprintw(maxy/4, (maxx-32)/2, "Enter the filename to  be read: ");
	scanw("%s", file);

	fp=fopen(file, "r");
	if(fp==NULL)
	{
		mvprintw(maxy/2, (maxx-22)/2, "Error opening file!!!");
		noecho();
		getch();
		endwin();
		return -1;
	}
	TT=0;
	while(!feof(fp))
	{
		i++;
		bt=(int *)realloc(bt, sizeof(int)*i);
		fscanf(fp, "%d", &bt[i-1]);
		TT+=bt[i-1];
	}
	TT-=bt[i-1];
	fclose(fp);
	n=i-1;
	
/****Gantt Chart****/
	getyx(stdscr, y, x);
	y+=4;
	x=(maxx-8)/2;
	WT=T=0;
	mvprintw(y, x, "G-Chart:");
	y+=2;
	unit=maxx/TT;
	x=(maxx-(TT*unit))/2;
	move(x, y);
	for(i=0; i<n; i++)
	{
		rectangle(y, x, y+2, x+(bt[i]*unit));
		mvprintw(y+1, x+(bt[i]*unit/2), "P%d", i+1);
		mvprintw(y+4, x, "%d", T);
		T+=bt[i];
		WT+=T;
		
		x+=(bt[i]*unit);
	}
	mvprintw(y+4, x, "%d", T);
	WT=(WT-T)/n;
	TAT=(float)TT/n + WT;
/****Avg TAT****/
	y+=8;
	mvprintw(y, (maxx-30)/2, "Average turn-around time: %.2f", TAT);
/****Avg WT****/
	y+=2;
	mvprintw(y, (maxx-30)/2, "Average waiting time: %.2f", WT);
	noecho();
	getch();
	endwin();
	return 0;
}

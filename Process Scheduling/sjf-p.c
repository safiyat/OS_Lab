#include <stdlib.h>
#include <curses.h>

#define min(x, y) x>y?y:x

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

void swap(int **queue, int a, int b)
{
	int unit;

	unit=queue[a][0];
	queue[a][0]=queue[b][0];
	queue[b][0]=unit;

	unit=queue[a][1];
	queue[a][1]=queue[b][1];
	queue[b][1]=unit;

	unit=queue[a][2];
	queue[a][2]=queue[b][2];
	queue[b][2]=unit;
}

void sort(int **queue, int a, int b, int param)
{
	int i, j;
	for(i=a; i<b-1; i++)
	{
		for(j=i; j<b; j++)
		{
			if(queue[i][param] > queue[j][param])
				swap(queue, i, j);
		}
	}

}

int main()
{
	int unit, proc, *preempt_times=NULL, i=0, j=0, preempt_count=1, CLOCK=0, N, TAT=0, WT=0, TT=0, **queue=NULL, prev_proc, prev_time, pq=0, x, y, maxx, maxy, Q, a, b, c, dec;
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
		queue=(int **)realloc(queue, sizeof(int *)*i);
		queue[i-1]=(int *)malloc(sizeof(int)*4);	//bT, arrT, finishT, PName

		queue[i-1][3]=i-1;
		fscanf(fp, "%d%d", &(queue[i-1][1]), &(queue[i-1][0]));	//ArrT, bT
//		printf("\n\n");
		TT+=queue[i-1][0];
	}
	fclose(fp);
	N=i-1;

	mvprintw((maxy/4)+1, (maxx-32)/2, "Enter the time quantum: ");
	scanw("%d", &Q);

/****Gantt Chart****/
	getyx(stdscr, y, x);
	y+=4;
	x=(maxx-8)/2;
	mvprintw(y, x, "G-Chart:");
	y+=2;
	unit=maxx/TT;
	x=(maxx-(TT*unit))/2;
//	mvprintw(y+1, x, "0");
	move(x, y);
	WT=TT=0;
	c=0;
	i=-1;
	CLOCK=0;
	while(c<N)
	{
		proc = 0;
		for(i=0; i<N; i++)
		{
			if(queue[i][0] == 9999 || CLOCK<queue[i][1])
				continue;
			if(queue[i][0] < queue[proc][0])
				proc = i;
		}
		dec=min(queue[proc][0], Q);

		rectangle(y, x, y+2, x+(dec*unit));
		mvprintw(y+1, x+(dec*unit/2), "P%d", queue[proc][3] + 1);
		mvprintw(y+4, x, "%d", CLOCK);

		CLOCK+=dec;
		queue[proc][0]-=dec;
		WT+=(N-c-1)*dec;
		if(queue[proc][0]==0)
		{
			TT+=CLOCK;
			queue[proc][0] = 9999;
			c++;
		}
		x+=(dec*unit);
	}
	mvprintw(y+4, x, "%d", CLOCK);
/****Avg TAT****/
	TAT=(float)TT/N;
	y+=8;
	mvprintw(y, (maxx-30)/2, "Average turn-around time: %.2f", TAT);
/****Avg WT****/
	WT/=N;
	y+=2;
	mvprintw(y, (maxx-30)/2, "Average waiting time: %.2f", WT);
	noecho();
	getch();
	endwin();
	return 0;
}

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

int main()
{
	int unit, proc, *preempt_times=NULL, i=0, j=0, preempt_count=1, CLOCK=0, N, TotalTTime=0, TotalWTime=0, TotalT=0, **queue=NULL, prev_proc, prev_time, pq=0, x, y, maxx, maxy;
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
	while(!feof(fp))
	{
		i++;
		queue=(int **)realloc(queue, sizeof(int *)*i);
		queue[i-1]=(int *)malloc(sizeof(int)*4);
		queue[i-1][0]=i;
		fscanf(fp, "%d%d%d", &(queue[i-1][1]), &(queue[i-1][0]), &(queue[i-1][3]));
		TotalT+=queue[i-1][0];
	}
	fclose(fp);
	N=i-1;
	unit=maxx/TotalT;

	getyx(stdscr, y, x);
	y+=4;
	x=(maxx-8)/2;
	mvprintw(y, x, "G-Chart:");
	y+=2;
	x=(maxx-(TotalT*unit))/2;
	i=0;
	prev_proc=0;
	prev_time=0;
	preempt_times=(int *)realloc(preempt_times, sizeof(int)*preempt_count);
	preempt_times[0]=0;
	preempt_count++;
	mvprintw(y+3, x, "0");
	while(i<N)
	{
		proc=0;
		pq=0;
		for(j=0; (j<(N))&&(queue[j][1]<=CLOCK); j++)
		{
			if(queue[j][0]==9999)
				continue;
			if(queue[j][3]<queue[proc][3])
				proc=j;
			pq++;
		}
		CLOCK++;
		queue[proc][0]--;
		TotalWTime=TotalWTime+pq-1;
		if(queue[proc][0]==0)
		{
			queue[proc][2]=CLOCK;
			TotalTTime+=CLOCK-queue[proc][1];
			queue[proc][0]=queue[proc][3]=9999;
			pq--;
			i++;
		}
		if(((prev_proc!=proc)||(i==N)))
		{
			if(prev_proc || CLOCK!=1)
			{
				rectangle(y, x, y+2, x+((CLOCK-prev_time)*unit));
				mvprintw(y+1, x+((CLOCK-prev_time)*unit/2), "P%d", prev_proc+1);
				if(CLOCK!=TotalT)
					mvprintw(y+3, x+((CLOCK-prev_time)*unit), "%d", CLOCK-1);
				else
					mvprintw(y+3, x+((CLOCK-prev_time)*unit), "%d", CLOCK);
				x+=((CLOCK-prev_time)*unit);
			}
			
			if(i==N)
			{
				rectangle(y, x, y+2, x+((CLOCK-prev_time)*unit));
				mvprintw(y+1, x+((CLOCK-prev_time)*unit/2), "P%d", proc+1);
				mvprintw(y+3, x+((CLOCK-prev_time)*unit), "%d", CLOCK);
			}

			preempt_times=(int *)realloc(preempt_times, sizeof(int)*preempt_count);
			preempt_times[preempt_count-1]=CLOCK-1;
			preempt_count++;

			prev_proc=proc;
			prev_time=CLOCK;
		}
	}
	
	
	y+=9;
	mvprintw(y, (maxx-30)/2, "Average turn-around time: %.2f", (float)TotalTTime/N);
	y+=2;
	mvprintw(y, (maxx-30)/2, "Average waiting time: %.2f", (float)TotalWTime/N);
	noecho();
	getch();
	endwin();
	return 0;
}

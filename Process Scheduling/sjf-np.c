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
	int unit, proc, i=0, j=0, CLOCK=0, N, TotalT=0, **queue=NULL, maxy, maxx, y, x;
	float TotalTTime=0, TotalWTime=0;
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
	TotalT=0;
	while(!feof(fp))
	{
		i++;
		queue=(int **)realloc(queue, sizeof(int *)*i);
		queue[i-1]=(int *)malloc(sizeof(int)*4);	//bT, arrT, finishT

		queue[i-1][0]=i;
		queue[i-1][3]=0;
		fscanf(fp, "%d%d", &(queue[i-1][2]), &(queue[i-1][1]));	//ArrT, bT
		TotalT+=queue[i-1][1];
	}
	fclose(fp);
	N=i-1;
	
	for(i=0; i<N; i++)
	{
		for(j=proc=i; (j<N)&&(queue[j][2]<=CLOCK); j++)
		{
			if(queue[j][1]<queue[proc][1])
				proc=j;
		}
		unit=queue[proc][0];
		queue[proc][0]=queue[i][0];
		queue[i][0]=unit;
		
		unit=queue[proc][1];
		queue[proc][1]=queue[i][1];
		queue[i][1]=unit;
		
		unit=queue[proc][2];
		queue[proc][2]=queue[i][2];
		queue[i][2]=unit;
		
		unit=queue[proc][3];
		queue[proc][3]=queue[i][3];
		queue[i][3]=unit;
		
		CLOCK+=queue[proc][1];
		queue[proc][3]=1;
	}


/****Gantt Chart****/
	getyx(stdscr, y, x);
	y+=4;
	x=(maxx-8)/2;
	mvprintw(y, x, "G-Chart:");
	y+=2;
	unit=maxx/TotalT;
	x=(maxx-(TotalT*unit))/2;
	move(x, y);
	TotalWTime=TotalT=0;
//	c=0;
	i=-1;
	CLOCK=0;
/*
	for(i=0; i<N; i++)
	{
		printf("[");
		for(j=0; j<(queue[i][1]*unit); j++)
		{
			if(j==queue[i][1]*unit/2)
				printf("P%d", queue[i][0]);
			printf("-");
		}
		printf("]");
	}
	printf("\n");
	CLOCK=0;
	printf("0   ");
	for(i=0; i<N; i++)
        {
                for(j=0; j<(queue[i][1]*unit); j++)
                {
                        printf(" ");
                }
		CLOCK+=queue[i][1];
		printf("%d  ", CLOCK);
		TotalWTime+=(CLOCK-queue[i][2]);
        }
****Avg WT****
	TotalWTime=(TotalWTime-CLOCK)/N;
	printf("\n\nAverage waiting time: %.2f", TotalWTime);
****Avg TAT****
	TotalTTime=(float)TotalT/N + TotalWTime;
	printf("\n\nAverage turn-around time: %.2f", TotalTTime);
	printf("\n");
	return 0;
*/
	for(i=0; i<N; i++)
	{
		rectangle(y, x, y+2, x+(queue[i][1]*unit));
		mvprintw(y+1, x+(queue[i][1]*unit/2), "P%d", queue[i][0]);
		mvprintw(y+4, x, "%d", CLOCK);
		CLOCK+=queue[i][1];
		TotalWTime+=(CLOCK-queue[i][2]);
		x+=(queue[i][1]*unit);
	}
	mvprintw(y+4, x, "%d", CLOCK);
	TotalWTime=(TotalWTime-CLOCK)/N;
	TotalTTime=(float)CLOCK/N + TotalWTime;
/****Avg TAT****/
	y+=8;
	mvprintw(y, (maxx-30)/2, "Average turn-around time: %.2f", TotalTTime);
/****Avg WT****/
	y+=2;
	mvprintw(y, (maxx-30)/2, "Average waiting time: %.2f", TotalWTime);
	noecho();
	getch();
	endwin();
	return 0;
}

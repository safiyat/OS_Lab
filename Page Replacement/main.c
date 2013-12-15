#include <curses.h>
#include <stdlib.h>
#include <string.h>

int maxy, maxx;

void rectangle(int y1, int x1, int y2, int x2)
{
	attron(A_BOLD);
	mvhline(y1, x1+1, 0, x2-x1);
	mvhline(y2, x1+1, 0, x2-x1);
	mvvline(y1+1, x1, 0, y2-y1);
	mvvline(y1+1, x2, 0, y2-y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
	attroff(A_BOLD);
}

int menu()
{
	int ch;
	do
	{
		getmaxyx(stdscr, maxy, maxx);
		werase(stdscr);
		attron(A_BOLD);
		mvprintw(maxy/2 - 4, (maxx - 16)/2, "Page Replacement");
		attroff(A_BOLD);
		mvprintw(maxy/2 - 2, maxx/2 - 4, "1. FIFO");
		mvprintw(maxy/2 - 1, maxx/2 - 4, "2. LRU");
		mvprintw(maxy/2, maxx/2 - 4, "3. Optimal");
		mvprintw(maxy/2 + 1, maxx/2 - 4, "4. Exit");
		mvprintw(maxy/2 + 3, maxx/2 - 8, "Enter your choice: ");
		ch = getch() - 48;
	}while(ch<0 || ch>5);
	return ch;
}

void fcfs(const char *requests, int frameSize)
{
	int i, j, x, y, k, faults;
	char *page, ch;
	getmaxyx(stdscr, maxy, maxx);
	werase(stdscr);
	i = 0;
	j = 0;
	faults = 0;
	y = (maxy - (frameSize + 3)) / 2;
	x = (maxx - (strlen(requests)*4 - 1)) / 2;
	page = (char *)malloc(sizeof(char) * (frameSize + 1));
	for(k = 0; k < frameSize; k++)
		page[k] = 0;
	page[frameSize - 1] = '\0';

	attron(A_BOLD);
	mvprintw( y - 2, (maxx - 18)/2, "First In First Out");
	attroff(A_BOLD);

	while(i < (strlen(requests) - 1))
	{
		i++;
		ch = requests[i-1];
		attron(A_BOLD);
		mvaddch(y, x + (i - 1)*4, ch);
		attroff(A_BOLD);

		if(strchr(page, ch) == NULL)
		{
			faults++;
			page[(j++)%frameSize] = ch;
			rectangle(y + 1, x + (i - 1)*4 + 1, y + frameSize +2, x + i*4 - 1);
			for(k = 0; k < frameSize; k++)
				if(page[k])
					mvaddch(y+k+2, x + i*4 - 2, page[k]);
		}
	}
	attron(A_BOLD);
	mvprintw( y + frameSize + 4, (maxx - 15)/2, "Page Faults: %d", faults);
	attroff(A_BOLD);
	getch();
}

void lru(const char *requests, int frameSize)
{
	int i, x, y, k, faults, min;
	char *page, ch, *used;
	getmaxyx(stdscr, maxy, maxx);
	werase(stdscr);
	i = 0;
	faults = 0;
	y = (maxy - (frameSize + 3)) / 2;
	x = (maxx - (strlen(requests)*4 - 1)) / 2;
	page = (char *)malloc(sizeof(char) * (frameSize + 1));
	used = (char *)malloc(sizeof(char) * (frameSize));
	for(k = 0; k < frameSize; k++)
	{
		page[k] = 0;
		used[k] = 0;
	}
	page[frameSize - 1] = '\0';

	attron(A_BOLD);
	mvprintw( y - 2, (maxx - 29)/2, "Least Recently Used");
	attroff(A_BOLD);

	while(i < (strlen(requests) - 1))
	{
		i++;
		ch = requests[i-1];
		attron(A_BOLD);
		mvaddch(y, x + (i - 1)*4, ch);
		attroff(A_BOLD);

		if(strchr(page, ch) == NULL)
		{
			faults++;

			for(k = 0, min = 0; k < frameSize; k++)
				if(used[k] < used[min])
					min = k;
			page[min] = ch;
			used[min] = i;
			rectangle(y + 1, x + (i - 1)*4 + 1, y + frameSize +2, x + i*4 - 1);
			for(k = 0; k < frameSize; k++)
				if(page[k])
					mvaddch(y+k+2, x + i*4 - 2, page[k]);
		}
		else
		{
			for(k = 0, min = 0; k < frameSize; k++)
				if(page[k] == ch)
					min = k;
			used[min] = i;
		}
	}
	attron(A_BOLD);
	mvprintw( y + frameSize + 4, (maxx - 15)/2, "Page Faults: %d", faults);
	attroff(A_BOLD);
	getch();
}

void optimal(const char *requests, int frameSize)
{
	int i, x, y, k, faults, count;
	char *page, ch, *p;
	getmaxyx(stdscr, maxy, maxx);
	werase(stdscr);
	i = 0;
	faults = 0;
	y = (maxy - (frameSize + 3)) / 2;
	x = (maxx - (strlen(requests)*4 - 1)) / 2;
	page = (char *)malloc(sizeof(char) * (frameSize + 1));
	for(k = 0; k < frameSize; k++)
		page[k] = 0;
	page[frameSize - 1] = '\0';

	attron(A_BOLD);
	mvprintw( y - 2, (maxx - 7)/2, "Optimal");
	attroff(A_BOLD);

	while(i < (strlen(requests) - 1))
	{
		i++;
		ch = requests[i-1];
		attron(A_BOLD);
		mvaddch(y, x + (i - 1)*4, ch);
		attroff(A_BOLD);

		if(strchr(page, ch) == NULL)
		{
			faults++;

			if(faults > frameSize)
			{
				for(k = i, count = 0; k < (strlen(requests) - 2); k++)
				{
					if((strchr(page, requests[k]))!=NULL)
						count++;
					else
						continue;
					if(count == frameSize)
						break;
				}
				p =strchr(page, requests[k]);
				*p = ch;
				rectangle(y + 1, x + (i - 1)*4 + 1, y + frameSize +2, x + i*4 - 1);
				for(k = 0; k < frameSize; k++)
					if(page[k])
						mvaddch(y+k+2, x + i*4 - 2, page[k]);
			}
			else
			{
				for(k = 0; k < frameSize; k++)
					if(page[k] == 0)
						break;
				page[k] = ch;
			}
		}
	}
	attron(A_BOLD);
	mvprintw( y + frameSize + 4, (maxx - 15)/2, "Page Faults: %d", faults);
	attroff(A_BOLD);
	getch();
}

int main(void)
{
	int frameSize, i = 0;
	char file[32], *requests = NULL, ch;
	FILE *fp;
	initscr();
	werase(stdscr);
	getmaxyx(stdscr, maxy, maxx);
	mvprintw(maxy/2 - 1, (maxx - 40)/2, "Enter the name of file to be read: ");
	scanw("%s", file);
	if((fp = fopen(file, "r")) == NULL)
	{
		mvprintw(maxy/2 + 1, (maxx - 20) / 2, "Error opening file!!!");
		getch();
		endwin();
		return -1;
	}
	ch = getc(fp);
	while(ch != EOF)
	{
		i++;
		requests = (char *) realloc(requests, sizeof(char) * i);
		requests[i - 1] = ch;
		ch = getc(fp);
	};
	requests = (char *) realloc(requests, sizeof(char) * (i+1));
	requests[i] = '\0';
	fclose(fp);

	mvprintw(maxy/2 + 1, (maxx - 30) / 2, "Enter the number of frames: ");
	scanw("%d", &frameSize);
	while(1)
	{
		switch(menu())
		{
			case 1: fcfs(requests, frameSize);
				break;

			case 2: lru(requests, frameSize);
				break;

			case 3: optimal(requests, frameSize);
				break;

			case 4: endwin();
				return 0;
		}
	}
	return 0;
}


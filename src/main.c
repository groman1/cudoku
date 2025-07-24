#include <ncurses.h>
#include <time.h>

void printBoard(char board[9][9], char uneditable[9][9])
{
	for (int y = 0; y<9; ++y)
	{
		for (int x = 0; x<9; ++x)
		{
			if (uneditable[y][x]) attron(A_REVERSE);
			mvprintw(y<<1, x<<1, "%c", board[y][x]);
			if (uneditable[y][x]) attroff(A_REVERSE);
		}
	}
}

void fillEmpty(char board[9][9], char character)
{
	for (int x = 0; x<9; ++x)
	{
		for (int y = 0; y<9; ++y)
		{
			board[x][y] = character;
		}
	}
}

// This function XOR's 1 into an 8 bit integer for each number from 2 to 9, if there are 8 numbers are there only one time then the ninth one is there too

int checkWin(char board[9][9])
{
	unsigned char checkhorizontal = 0, checkvertical = 0, checkcell = 0;
	for (int i = 0; i<9; ++i, checkhorizontal = 0, checkvertical = 0, checkcell = 0)
	{
		for (int x = 0; x<9; ++x)
		{
			checkhorizontal ^= 1<<(board[i][x]-50);
			checkvertical ^= 1<<(board[x][i]-50);
			checkcell ^= 1<<(board[i/3+x%3][x/3+(i/3)*3]-50);
		}
		if (checkhorizontal!=0xFF||checkvertical!=0xFF||checkcell!=0xFF) return 0;
	}
	return 1;
}

void generateBoard(char board[9][9], char uneditable[9][9], int qtyOpened)
{
	struct timespec ts1, ts2;
	int checkvalue,	position;
	clock_gettime(CLOCK_REALTIME, &ts2);
	for (int i = 0; i<qtyOpened;)
	{
		clock_gettime(CLOCK_REALTIME, &ts1);
		
		checkvalue = (ts1.tv_nsec&ts2.tv_nsec)%9+1;
		position = (ts1.tv_nsec^ts2.tv_nsec)%81;
		if (board[position/9][position%9]!=32) continue;

		for (int x = 0; x<9; ++x)
		{
			if (board[x][position%9]-48==checkvalue||board[position/9][x]-48==checkvalue||board[position/3+x/3][x%3+(position/3*3)]-48==checkvalue) 
			{	checkvalue = 0; break;	}
		}
		if (checkvalue) 
		{
			board[position/9][position%9] = checkvalue+48; 
			uneditable[position/9][position%9] = 1;
			++i;
		}

		clock_gettime(CLOCK_REALTIME, &ts2);
	}
}

int main(int argc, char **argv)
{
	char board[9][9], uneditable[9][9];
	int maxx, maxy, ch, x, y, qtyOpened = 0, selected;
	
	if (argc==2)
	{
		for (int i = 0; argv[1][i]; ++i)
		{
			qtyOpened*=10;
			qtyOpened+=argv[1][i]-48;
		}
		if (qtyOpened>64) 
		{ return 1; }
	}
	else qtyOpened = 16;

	initscr();
	noecho();
	keypad(stdscr, 1);
	getmaxyx(stdscr, maxy, maxx);
	if (maxx<18||maxy<20) { endwin(); return 1; }
restart:
	x = y = 0;
	selected = qtyOpened;
	fillEmpty(board, 32);
	fillEmpty(uneditable, 0);
	generateBoard(board, uneditable, qtyOpened);
	printBoard(board, uneditable);
	move(0,0);
	
	while((ch=getch())!='q')
	{
		switch (ch)
		{
			case '1'...'9': 
			{ if (uneditable[y][x]==0) { if (board[y][x]==32) { ++selected; } board[y][x] = ch; printBoard(board, uneditable); } break; }
			case 260: 
			{ if (x) { --x; } break; }
			case 261:
			{ if (x<8) { ++x; } break; }
			case 259:
			{ if (y) { --y; } break; }
			case 258:
			{ if (y<8) { ++y; } break; }
			case 263: case '0':
			{ if (uneditable[y][x]==0) { board[y][x] = 32; printBoard(board, uneditable); } break; }
			case 'r': case 'R':
			{ x = y = 0; goto restart; }
			default: break;
		}
		move(y<<1,x<<1);
		if (selected==81)
		{
			if (checkWin(board)) 
			{
				mvprintw(19,0, "Congratulations!\nPress r to restart");
				if (getch()=='r') goto restart;
				else break;
			}
		}
	}
	endwin();
	return 0;
}

#ifndef _GRAPH_H_
#define _GRAPH_H_

#pragma warning(disable: 4996)

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#include "snake.h"

//screen size
const int LENGTH = 155;
const int WIDTH = 43;
//snake area
const int ROW = 40;
const int COLUMN = 100;
const int MIDDLE = 26;
//controls
const char UP		= 'H';
const char DOWN		= 'P';
const char LEFT		= 'K';
const char RIGHT	= 'M';
const char STOP		= 'S';
const char ESC		= (char)27;
const char ENTER	= '\n';
//cheat controls
const char DECELERATE	= 'D';
const char ACCELERATE	= 'A';
const char UPGRADE		= 'U';
//graphs
const char BAR		= '#';
const char LINE		= '-';
const char POINTER	= '>';
const char SNAKE	= '*';
const char SPACE	= ' ';
const char FOOD		= '$';
//texts
const char GAME_NAME[]	= "Tony's Snake 2.2 Remastered";
const char NOTE[]		= "Programmed by Tony";
const char TIME[]		= "2021.9.13";
const char NOTE2[]		= "Thanks for your playing!";
const char NOTE3[]		= "Cannot find the file \"help.txt\" !\a";
const char NOTE4[]		= "Press any key to continue.";
const char OVER[]		= "GAME OVER !\a";
const char TIRED1[]		= "You've played so many times!";
const char TIRED2[]		= "Let's have a rest!";
const char SAVE[]		= "Save the game and continue?\a";
const char LOAD_ERROR[] = "The game archive is DAMAGED!\a";

void gotoPosition(int x, int y);
void hideCursor();
void showCursor();
int getCommand();
void clearScreen();
void clearScreen(int delay);
void setTextColor(WORD color);	//set current text color
void setTextFont(SHORT width, SHORT height, const wchar_t* font);	//set current text font
void generate();
void gameInterface();
void printScore(int current, int high);
void printData(int sx, int sy, int length, int fx, int fy, int delay);
void menuInterface(bool showContinue);
void errorInterface();
void absDelay(DWORD milliseconds);

void gotoPosition(int x, int y)
{
	HANDLE Output;
	COORD Position;
	
	Position.X = x;
	Position.Y = y;
	Output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(Output, Position);

	return ;
}
void hideCursor()
{
	HANDLE Output;
	CONSOLE_CURSOR_INFO Cursor;
	
	Output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(Output, &Cursor);
	Cursor.bVisible = false;
	SetConsoleCursorInfo(Output, &Cursor);
}
void showCursor()
{
	HANDLE Output;
	CONSOLE_CURSOR_INFO Cursor;
	
	Output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(Output, &Cursor);
	Cursor.bVisible = true;
	SetConsoleCursorInfo(Output, &Cursor);
}
int getCommand()
{
	char rv;

	while (1)
	{
		if (_kbhit())
		{
			rv = toupper(_getch());
			break;
		}
	}
	
	return rv;
}
void clearScreen()
{
	system("CLS");

	return ;
}
void clearScreen(int delay)
{
	for (int i = LENGTH - 1; i >= 0; i--)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			gotoPosition(i, j);
			putchar(SPACE);
			absDelay(delay);
		}
	}
}
void setTextColor(WORD color)
{
	HANDLE hOutput;

	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOutput, color);
}
void setTextFont(SHORT width, SHORT height, const wchar_t* font)
{
	CONSOLE_FONT_INFOEX console_font_info;
	HANDLE hOutput;

	console_font_info.cbSize = sizeof console_font_info;
	console_font_info.nFont = 0;
	console_font_info.dwFontSize.X = width;
	console_font_info.dwFontSize.Y = height;
	console_font_info.FontFamily = FF_DONTCARE;
	console_font_info.FontWeight = FW_NORMAL;
	wcscpy(console_font_info.FaceName, font);

	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hOutput, false, &console_font_info);
}
void generate()
{
	clearScreen();
	system("MODE CON LINES=42 COLS=155");

	return ;
}
void gameInterface()
{
	int i, j;
	
	clearScreen();
	//snake's area
	for (i = 1; i <= COLUMN; i++)
	{
		for (j = 1; j <= ROW; j++)
		{
			if ((i == 1) || (i == COLUMN) || (j == 1) || (j == ROW))
			{
				gotoPosition(i, j);
				putchar(BAR);
			}
		}
	}
	//break lines
	for (i = COLUMN + 1, j = 1; i < LENGTH - 1 ; i++)
	{
		gotoPosition(i, j);
		putchar(LINE);
	}
	for (i = COLUMN + 1, j = MIDDLE; i < LENGTH - 1; i++)
	{
		gotoPosition(i, j);
		putchar(LINE);
	}
	for (i = COLUMN + 1, j = ROW; i < LENGTH - 1; i++)
	{
		gotoPosition(i, j);
		putchar(LINE);
	}
	//notes
	setTextColor(0xD);
	i = COLUMN + (LENGTH - COLUMN - strlen(GAME_NAME)) / 2;
	j = MIDDLE + 4;
	gotoPosition(i, j);
	printf("%s", GAME_NAME);
	i = COLUMN + (LENGTH - COLUMN - strlen(NOTE)) / 2;
	j += 4;
	gotoPosition(i, j);
	printf("%s", NOTE);
	i = COLUMN + (LENGTH - COLUMN - strlen(TIME)) / 2;
	j += 2;
	gotoPosition(i, j);
	printf("%s", TIME);
	setTextColor(0x7);

	return ;
}
void printScore(int current, int high)
{
	int i, j;
	int level;

	level = current / 10;
	if (level > 10)
		level = 10;

	i = COLUMN + (LENGTH - COLUMN - 17) / 2;
	j = (ROW - MIDDLE) * 4 / 10 - 2;
	
	gotoPosition(i, j);
	printf("Level: %d", level);
	if (level == 10)
		printf(" maximum !");
	j += 2;
	gotoPosition(i, j);
	if (current > high)
		setTextColor(0xA);
	printf("Your score: %5d", current);
	setTextColor(0x7);
	j += 2;
	gotoPosition(i, j);
	printf("High score: %5d", high);

	return ;
}
void printData(int sx, int sy, int length, int fx, int fy, int delay)
{
	int i, j;

	setTextColor(0x8);
	i = COLUMN + (LENGTH - COLUMN - 26) / 2 - 2;
	j = (ROW - MIDDLE) * 6 / 10 + 2;
	//snake
	gotoPosition(i, j);
	printf("Snake:");
	j += 2;
	gotoPosition(i, j);
	printf("\tposition:\t (%3d, %3d)", sx, sy);
	j += 2;
	gotoPosition(i, j);
	printf("\tlength:\t %d", length);
	//food
	j += 3;
	gotoPosition(i, j);
	printf("Food:");
	j += 2;
	gotoPosition(i, j);
	printf("\tposition:\t (%3d, %3d)", fx, fy);
	j += 3;
	gotoPosition(i, j);
	printf("Delay: %4d ms", delay);
	setTextColor(0x7);
}
void menuInterface(bool showContinue)
{
	int i, j;

	clearScreen();
	gameInterface();

	i = (COLUMN - strlen(GAME_NAME) - 11) / 2;
	j = ROW - MIDDLE;
	gotoPosition(i, j);
	setTextColor(0x2);
	printf("Welcome to %s", GAME_NAME);
	setTextColor(0x7);

	i = (COLUMN - 7) / 2;
	j += 5;
	gotoPosition(i, j);
	printf("Play(P)");

	if (showContinue)
	{
		i = (COLUMN - 11) / 2;
		j += 2;
		gotoPosition(i, j);
		printf("Continue(C)");
	}

	i = (COLUMN - 7) / 2;
	j += 2;
	gotoPosition(i, j);
	printf("Help(H)");

	i = (COLUMN - 10) / 2;
	j += 3;
	gotoPosition(i, j);
	setTextColor(0x8);
	printf("Quit(Esc)");
	setTextColor(0x7);

	return ;
}
void errorInterface()
{
	int i, j;

	clearScreen();
	gameInterface();

	i = (COLUMN - strlen(GAME_NAME) - 11) / 2;
	j = ROW / 2;
	gotoPosition(i, j);
	setTextColor(0x4);
	printf("%s", LOAD_ERROR);
	setTextColor(0x7);
	absDelay(2000);
}
void absDelay(DWORD milliseconds)
{
	static DWORD old_time = GetTickCount();

	while (GetTickCount() - old_time < milliseconds)
		continue;
	old_time = GetTickCount();
}

#endif

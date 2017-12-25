// 100layers.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*Man Down*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

/*****************************控制台颜色函数宏定义*****************************/
#define RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED)
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN)
#define BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE)
#define GREENPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define REDPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY)
#define BLUEPLUS SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define YELLO SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY)
#define INDIG SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define MagentaPlus SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define White SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define Other SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED&FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)

/*长宽大小*/
#define MAX 15
#define BoardLEN 5

/*踏板类型宏定义数值*/
#define Sinpe 0
#define Hard 1
#define Fragile 2
#define Move 3
#define HP_PLUS 4

void HideCursor(void);
void Start(void); /*开始选择*/
void gotoxy(int, int);
bool Menu(void); /*选择菜单*/
void Begin(void); /*初始化场景*/
void RandBoard(int); /*随机生成踏板*/
void Play(void); /*玩家操作*/
void Display(void); /*显示*/
unsigned int FindMax(void); /*找出最下面的踏板的y坐标*/
bool ManMoveDetectionA(void); /*是否进行移动的检测*/
bool ManMoveDetectionD(void);
bool CoordinateComparisonOfMoving(void); /*碰撞踏板的检测*/
int CollisionDetection(void); /*踩踏碰撞检测*/
void GenerationDetection(void); /*生成函数*/
void SomethingWillHappen(int); /*踩踏板事件生成*/
bool IsDieDetection(void); /*死亡检测*/

int HP = 100; /*生命*/
int Score = 0; /*分数*/
int Times_ = 0;
unsigned long Speed = 500;

/*人物位置信息*/
struct MAN
{
	unsigned int x;
	unsigned int y;
	bool Stop;
} Man;

/*游戏场景中的踏板*/
struct BOARD
{
	unsigned int leftx;
	unsigned int rightx;
	unsigned int y;
	int style;
} Board[4];

int main(void)
{
	HideCursor();
	Start();
THE_ORIGIN_SAGA:
	srand((unsigned)(time(nullptr)));
	Begin();
	Play();
	if (Menu())
	{
		goto THE_ORIGIN_SAGA;
	}
	return 0;
}

void Start(void)
{
	char ch;
	int speed_temp;
	printf("********************It's a man who down one hundred layers********************\n\n");
	printf("                          a.Start game.\n");
	printf("                          b.Introduction.\n");
	printf("                          c.Setting difficulty.\n\n");
N:
	scanf_s("%c", &ch);
	switch (ch)
	{
	case 'B':
	case 'b':
		{
			system("cls");
			printf("Direction control:  ←:A  →:D  \n\n");
			printf("Type of stone:\n");
			REDPLUS;
			printf("^^^^^^");
			printf("Spike\n\n");
			BLUEPLUS;
			printf("------");
			printf("Common slate\n\n");
			YELLO;
			printf("~~~~~~");
			printf("Fragile stone plate\n\n");
			White;
			printf("******");
			printf("Conveyor belt\n\n");
			GREENPLUS;
			printf("++++++");
			printf("  Blood slate\n\n");
			printf("Play: the upper spike moves downward continuously by controlling the game player characters, to avoid being injured and avoid falling into the abyss of thorns.\n");
			getchar();
			getchar();
			break;
		}
	case 'A':
	case 'a': return;
	case 'C':
	case 'c':
		{
			do
			{
				printf("Please enter the interval speed (millisecond):");
				scanf_s("%d", &speed_temp);
			}
			while (speed_temp < 0);
			Speed = (unsigned int)(speed_temp);
			break;
		}
	default: goto N;
	}
}

/**
* 游戏结束之后的菜单选择.
*/

bool Menu(void)
{
	char ch;
	White;
	system("cls");
	printf("Game over! \nscore:%d\n\n", Score);
	printf("Cumulative number of stone formation:%d\n\n", Times_ + 4);
	printf("\n\nContinue? (Y key continues, and the rest of the key exit):");
	scanf_s("%c", &ch);
	return (ch == 'Y' || ch == 'y');
}

/**
* 初始化一些游戏场景数据.
*/
void Begin(void)
{
	Man.x = 5;
	Man.y = 5;
	Man.Stop = false;
	RandBoard(0);
	Board[0].y = 1;
	RandBoard(1);
	Board[1].y = 5;
	RandBoard(2);
	Board[2].y = 9;
	RandBoard(3);
	Board[3].y = 13;
	Times_++;
}

/**
* 随机生成一个“石板”信息填充到指定结构.
* @N 被填充结构的下标.
*/
void RandBoard(int N)
{
	do
	{
		Board[N].leftx = rand() % MAX;
		Board[N].rightx = Board[N].leftx + (BoardLEN - 1);
	}
	while (Board[N].rightx > MAX);
	Board[N].style = rand() % 5;
	Board[N].y = FindMax() + 4;
	Times_++;
}

void Play(void)
{
	char ch;
	int num; /*获取玩家正在站立的木板的位置*/
	unsigned int count;
	while (true)
	{
		while (_kbhit())
		{
			ch = _getch();
			switch (ch)
			{
			case 'a':
			case 'A':
				{
					if (!ManMoveDetectionA())
					{
						Man.x--;
					}
					break;
				}
			case 'd':
			case 'D':
				{
					if (!ManMoveDetectionD())
					{
						Man.x++;
					}
					break;
				}
			default: ;
			}
		}
		system("cls");
		GenerationDetection();
		num = CollisionDetection();
		SomethingWillHappen(num);
		for (count = 0; count < 4; count++)
		{
			Board[count].y--;
		}
		if (!Man.Stop)
		{
			(Man.y)++;
		}
		else
		{
			Man.y = Board[num].y - 1;
		}

		if (IsDieDetection())
		{
			return;
		}
		Display();
		Score++;
		Sleep((DWORD)(Speed));
	}
}

/**
* 检测玩家是否站到了石板上，如果是，在Man结构中的Stop标记为true.
* @return 如果是，返回该石板结构下标.如果不是，返回-1.
*/
int CollisionDetection(void)
{
	int x, y, num;
	for (x = 0; x < MAX; x++)
	{
		for (y = 0; y < MAX; y++)
		{
			for (num = 0; num < 4; num++)
			{
				if (Man.y + 2 == Board[num].y || Man.y + 1 == Board[num].y)
				{
					if (Man.x + 2 >= Board[num].leftx && Man.x <= Board[num].rightx)
					{
						Man.Stop = true;
						return num;
					}
				}
			}
		}
	}
	Man.Stop = false;
	return -1;
}

/**
* 显示游戏场景.
*/
void Display(void)
{
	int N;
	gotoxy(Man.x + 10, Man.y + 1);
	White;
	printf("QAQ");
	gotoxy(20 + 10, 0 + 1);
	printf("HP:%d", HP);
	gotoxy(20 + 10, 1 + 1);
	printf("Score:%d", Score);
	gotoxy(10, MAX + 1);
	INDIG;
	printf("---------------");
	gotoxy(10, 0);
	White;
	printf("^^^^^^^^^^^^^^^");
	for (N = 0; N < 4; N++)
	{
		if (Board[N].y > MAX)
		{
			continue;
		}
		gotoxy(Board[N].leftx + 10, Board[N].y + 1);
		switch (Board[N].style)
		{
		case Sinpe:
			{
				REDPLUS;
				printf("^^^^^^");
				break;
			}
		case Hard:
			{
				BLUEPLUS;
				printf("------");
				break;
			}
		case Fragile:
			{
				YELLO;
				printf("~~~~~~");
				break;
			}
		case Move:
			{
				White;
				printf("******");
				break;
			}
		case HP_PLUS:
			{
				GREENPLUS;
				printf("++++++");
				break;
			}
		default: ;
		}
	}
}

bool ManMoveDetectionA(void)
{
	return (Man.x > 0 && CoordinateComparisonOfMoving());
}

bool ManMoveDetectionD(void)
{
	return (Man.x > 0 && CoordinateComparisonOfMoving());
}

bool CoordinateComparisonOfMoving(void)
{
	int x, y, num;
	for (x = 0; x < MAX; x++)
	{
		for (y = 0; y < MAX; y++)
		{
			for (num = 0; num < 4; num++)
			{
				if (Man.x + 1 + 2 == Board[num].leftx && Man.y == Board[num].y)
				{
					return true;
				}
				if (Man.x == Board[num].rightx && Man.y == Board[num].y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GenerationDetection(void)
{
	int count;
	for (count = 0; count < 4; count++)
	{
		if (Board[count].y == 0)
		{
			RandBoard(count);
			return;
		}
	}
}

unsigned int FindMax(void)
{
	unsigned int Tmp, Tmp2;
	Tmp = max(Board[0].y, Board[1].y);
	Tmp2 = max(Board[2].y, Board[3].y);
	return max(Tmp, Tmp2);
}

void SomethingWillHappen(int num)
{
	if (num >= 0)
	{
		switch (Board[num].style)
		{
		case Sinpe:
			{
				HP -= 20;
				break;
			}
		case Fragile:
			{
				RandBoard(num);
				Man.Stop = false;
				break;
			}
		case Move:
			{
				Man.x++;
				break;
			}
		case HP_PLUS:
			{
				HP += 20;
				if (HP > 100)
				{
					HP = 100;
				}
				break;
			}
		default: ;
		}
	}
}

bool IsDieDetection(void)
{
	if (HP <= 0 || Man.y <= 0 || Man.y >= MAX)
	{
		return true;
	}
	return false;
}

void gotoxy(int x, int y)
{
	COORD pos = {x, y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void HideCursor(void)
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
